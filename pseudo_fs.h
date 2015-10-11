/*
 * ProcFs.h
 *
 *  Created on: Aug 27, 2014
 *      Author: ed
 * (c) 2014, WigWag Inc
 */
#ifndef PSEUDO_FS_H
#define PSEUDO_FS_H

#include "nan.h"

#include <v8.h>
#include <node.h>
#include <uv.h>
#include <node_buffer.h>
//#include "node_pointer.h"
//#include "network-common.h"

using namespace node;
using namespace v8;

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <endian.h>

#include <string.h>
#include <stdlib.h>
#include <uv.h>

// should be bigger than the normal MTU
#define READ_DEFAULT_CHUNK_SIZE  5000
#define DEFAULT_RETRIES 4       // number of times to try if a file is not writing out fully
#define TIMEOUT_FOR_RETRY 1000  // in ms


#if (UV_VERSION_MAJOR < 1)
#define USE_UV_REF 1
#endif

/**
 * LICENSE_IMPORT_BEGIN 9/7/14
 *
 * Macros below pulled from this project:
 *
 * https://github.com/bnoordhuis/node-buffertools/blob/master/buffertools.cc
 *
 * and include additions by WigWag.
 *
 * original license:

Copyright (c) 2010, Ben Noordhuis <info@bnoordhuis.nl>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#if NODE_MAJOR_VERSION > 0 || NODE_MINOR_VERSION > 10
# define UNI_BOOLEAN_NEW(value)                                               \
    v8::Boolean::New(info.GetIsolate(), value)
# define UNI_BUFFER_NEW(size)                                                 \
    node::Buffer::New(info.GetIsolate(), size)
# define UNI_CONST_ARGUMENTS(name)                                            \
    const v8::FunctionCallbackInfo<v8::Value>& name
# define UNI_ESCAPE(value)                                                    \
    return handle_scope.Escape(value)
# define UNI_ESCAPABLE_HANDLESCOPE()                                          \
    v8::EscapableHandleScope handle_scope(args.GetIsolate())
# define UNI_FUNCTION_CALLBACK(name)                                          \
    void name(const v8::FunctionCallbackInfo<v8::Value>& args)
# define UNI_HANDLESCOPE()                                                    \
    v8::HandleScope handle_scope(args.GetIsolate())
# define UNI_INTEGER_NEW(value)                                               \
    v8::Integer::New(args.GetIsolate(), value)
# define UNI_RETURN(value)                                                    \
    args.GetReturnValue().Set(value)
# define UNI_STRING_EMPTY()                                                   \
    v8::String::Empty(args.GetIsolate())
# define UNI_STRING_NEW(string, size)                                         \
    v8::String::NewFromUtf8(args.GetIsolate(),                                \
                            string,                                           \
                            v8::String::kNormalString,                        \
                            size)
# define UNI_THROW_AND_RETURN(type, message)                                  \
    do {                                                                      \
      args.GetIsolate()->ThrowException(                                      \
          type(v8::String::NewFromUtf8(args.GetIsolate(), message)));         \
      return;                                                                 \
    } while (0)
# define UNI_THROW_EXCEPTION(type, message)                                   \
    args.GetIsolate()->ThrowException(                                        \
        type(v8::String::NewFromUtf8(args.GetIsolate(), message)));
#else  // NODE_MAJOR_VERSION > 0 || NODE_MINOR_VERSION > 10
# define UNI_BOOLEAN_NEW(value)                                               \
    v8::Local<v8::Boolean>::New(v8::Boolean::New(value))
# define UNI_BUFFER_NEW(size)                                                 \
    v8::Local<v8::Object>::New(node::Buffer::New(size)->handle_)
# define UNI_CONST_ARGUMENTS(name)                                            \
    const v8::Arguments& name
# define UNI_ESCAPE(value)                                                    \
    return handle_scope.Close(value)
# define UNI_ESCAPABLE_HANDLESCOPE()                                          \
    v8::HandleScope handle_scope
# define UNI_FUNCTION_CALLBACK(name)                                          \
    v8::Handle<v8::Value> name(const v8::Arguments& args)
# define UNI_HANDLESCOPE()                                                    \
    v8::HandleScope handle_scope
# define UNI_INTEGER_NEW(value)                                               \
    v8::Integer::New(value)
# define UNI_RETURN(value)                                                    \
    return handle_scope.Close(value)
# define UNI_STRING_EMPTY()                                                   \
    v8::String::Empty()
# define UNI_STRING_NEW(string, size)                                         \
    v8::String::New(string, size)
# define UNI_THROW_AND_RETURN(type, message)                                  \
    return v8::ThrowException(v8::String::New(message))
# define UNI_THROW_EXCEPTION(type, message)                                   \
    v8::ThrowException(v8::String::New(message))
#endif  // NODE_MAJOR_VERSION > 0 || NODE_MINOR_VERSION > 10

// LICENSE_IMPORT_END



const int MAX_IF_NAME_LEN = 16;

class PseudoFs : public Nan::ObjectWrap {
protected:

//	static void do_create(uv_work_t *req);
//	static void post_checkout(uv_work_t *req, int status);
//
//	static void do_clone(uv_work_t *req);
//	static void master_work(uv_work_t *req);
//
//	static void post_clone(uv_work_t *req, int status);
//	static void post_master(uv_work_t *req, int status);

//	char _if_name[MAX_IF_NAME_LEN+1];
	char *_err_str;
	char *_path;
	int _fd;
	int _fs_flags;  // file system open flags
	int _fs_error;

	int read_chunk_size;

	v8::Persistent<Function> onDataCB;

//	struct readReq {
//		uv_work_t work;
//		int _errno; // the errno that happened on read if an error ocurred.
//		v8::Persistent<Object> buffer;
////		v8::Persistent<String> buffer;
//		char *_path;
//		char *_backing;                // the backing store of the buffer
//		int len;
//		PseudoFs *self;
//		// need Buffer
//		readReq(PseudoFs *i) : _errno(0), completeCB(), buffer(), _path(NULL), _backing(NULL), len(0), self(i) {
//			work.data = this;
//		}
//		readReq() = delete;
//	};

	struct chunk {
		char *_buf;
		int _size; // size of memory
		int _len;  // the amount of the chunk used
		chunk *_next;
		chunk(int s) : _buf(NULL), _size(s), _len(0), _next(NULL) {
			_buf = (char *) malloc(_size);
		}
		chunk() = delete;
	};

	enum workType : unsigned int { READ = 0x01, WRITE = 0x02, OPEN = 0x04, CLOSE = 0x08, SSHOT = 0x10 }; // SSHOT = single shot, only read once (for stream)

	struct workReq {
		uv_work_t work;
		uv_timer_t timeoutHandle; // used to hold the event loop from exiting
		bool ref;
		unsigned int t;
		int _fd;
		int _fs_flags;
		int _errno;     // the errno that happened on read if an error occurred.
		Nan::Callback *completeCB;
		Nan::Callback *onSendSuccessCB;
		Nan::Callback *onSendFailureCB;
		Nan::Persistent<Object> buffer; // Buffer object passed in
		char *_backing;    // backing of the passed in Buffer
		bool freeBacking;  // free the backing on delete?
		int len; // amount read or written
		PseudoFs *self;
		int _reqSize;
		chunk *_extras; // this is a linked list of extra chunks (if the original read is too small - ideally we rarely use this)
		int retries;
		int timeout;
		// need Buffer
		workReq(PseudoFs *i, unsigned int _t) : ref(true), t(_t), _fd(0), _fs_flags(0), _errno(0), completeCB(NULL), onSendSuccessCB(NULL), onSendFailureCB(NULL), buffer(),
				_backing(NULL), freeBacking(false), len(0), self(i), _reqSize(0),
				_extras(NULL), retries(DEFAULT_RETRIES), timeout(TIMEOUT_FOR_RETRY) {
			work.data = this;
		}
		workReq() = delete;
		~workReq() {
			chunk *n = _extras;
			while(n) {
				if(n->_buf) free(n->_buf);
				chunk *b = n;
				n = n->_next;
				delete b;
			}
			if(freeBacking) ::free(_backing);
		}
	};

//	static void do_read(uv_work_t *req);
//	static void post_read(uv_work_t *req, int status);
//
//	static void do_write(uv_work_t *req);
//	static void post_write(uv_work_t *req, int status);
	static void do_work(uv_work_t *req);
	static void post_work(uv_work_t *req, int status);
#if (UV_VERSION_MAJOR > 0)
	static void timercb_pseudofs(uv_timer_t *h);
#else
	static void timercb_pseudofs(uv_timer_t *h, int status);
#endif
	static void uv_close_handle_cb(uv_handle_t* handle);
public:
//	static Handle<Value> Init(const Arguments& args);
    static NAN_METHOD(Init);

//	static void ExtendFrom(const Arguments& args);
//    static void Shutdown();
    static NAN_METHOD(ExtendFrom);
    static NAN_METHOD(Shutdown);



    static Nan::Persistent<Function> constructor_template;
//    static Handle<Value> New(const Arguments& args);
    static NAN_METHOD(New);

//    static Handle<Value> New(const Arguments& args);
//    static Handle<Value> NewInstance(const Arguments& args);

//    static Handle<Value> IsCreated(const Arguments& args);

//    static Handle<Value> GetIfName(Local<String> property, const AccessorInfo &info);
//    static void SetIfName(Local<String> property, Local<Value> val, const AccessorInfo &info);
//    static Handle<Value> GetIfFD(Local<String> property, const AccessorInfo &info);
//    static void SetIfFD(Local<String> property, Local<Value> val, const AccessorInfo &info);
//    static Handle<Value> GetIfFlags(Local<String> property, const AccessorInfo &info);
//    static void SetIfFlags(Local<String> property, Local<Value> val, const AccessorInfo &info);
//    static Handle<Value> GetLastError(Local<String> property, const AccessorInfo &info);
//    static void SetLastError(Local<String> property, Local<Value> val, const AccessorInfo &info);
//    static Handle<Value> GetLastErrorStr(Local<String> property, const AccessorInfo &info);
//    static void SetLastErrorStr(Local<String> property, Local<Value> val, const AccessorInfo &info);

//    static Handle<Value> GetReadChunkSize(Local<String> property, const AccessorInfo &info);
//    static void SetReadChunkSize(Local<String> property, Local<Value> val, const AccessorInfo &info);
    void GetReadChunkSize(v8::Local<v8::String> property,
                            const Nan::PropertyCallbackInfo<v8::Value>& info);

    void SetReadChunkSize(v8::Local<v8::String> property,
                            v8::Local<v8::Value> value,
                            const Nan::PropertyCallbackInfo<v8::Value>& info);

//    static Handle<Value> ReadPseudofile(const Arguments& args);
//    static Handle<Value> WritePseudofile(const Arguments& args);
    static NAN_METHOD(ReadPseudofile);
    static NAN_METHOD(WritePseudofile);


//    static Handle<Value> Close(const Arguments& args);


//    static Handle<Value> Create(const Arguments& args);
//    static Handle<Value> Open(const Arguments& args);
//    static Handle<Value> Close(const Arguments& args);


    static Nan::Persistent<Function> constructor;
    static Nan::Persistent<ObjectTemplate> prototype;

    PseudoFs(char *path = NULL) :
    	_err_str(NULL),
    	_path(NULL),
    	_fd(0), _fs_flags(), _fs_error(0),
    	read_chunk_size(READ_DEFAULT_CHUNK_SIZE),
    	onDataCB()
    	{
    		_path = ::strdup(path);
    	}

    ~PseudoFs() {
    	if(_path) free(_path);
    	if(_err_str) free(_err_str);
    }

	void setErrStr(char *zSprefix, char *zStr) {
		if(_err_str) free(_err_str);
		_err_str = NULL;
		if(zStr) {
			if(zSprefix) {
				int total = strlen(zStr) + strlen(zSprefix) + 1;
				_err_str = (char *) malloc(total);
				strcpy(_err_str, zSprefix);
				strcat(_err_str, zStr);
			} else
				_err_str = strdup(zStr);
		}
	}

};


#endif /* PSUEDO_FS_H */
