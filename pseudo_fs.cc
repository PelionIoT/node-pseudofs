/*
 * ProcFs.cc
 *
 *  Created on: Aug 27, 2014
 *      Author: ed
 * (c) 2014, Framez Inc
 */


#include "pseudo_fs.h"
#define _ERRCMN_ADD_CONSTS 1
#include "error-common.h"

// BUILDTYPE is a node-gyp-dev thing
#ifdef PSEUDOFS_DEBUG_BUILD
#warning "*** Debug build."
#endif

#include "pseudo_fs_err.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

using namespace v8;


//int ProcFs::tun_create() {
//
//  struct ifreq ifr;
//  int _err;
//  const char *clonedev = "/dev/net/tun";
//
//  /* Arguments taken by the function:
//   *
//   * char *dev: the name of an interface (or '\0'). MUST have enough
//   *   space to hold the interface name if '\0' is passed
//   * int flags: interface flags (eg, IFF_TUN etc.)
//   */
//
//
//
//   /* open the clone device */
//   if( (_if_fd = open(clonedev, O_RDWR)) < 0 ) {
//	   _if_fd = 0;
//   	   _if_error = errno;
//   	   setErrStr("open(): ", strerror(errno));
//	   return 0;
//   }
//
//   /* preparation of the struct ifr, of type "struct ifreq" */
//   memset(&ifr, 0, sizeof(ifr));
//
//   ifr.ifr_flags = _if_flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */
//
//   if (*_if_name) {
//     /* if a device name was specified, put it in the structure; otherwise,
//      * the kernel will try to allocate the "next" device of the
//      * specified type */
//	   strncpy(ifr.ifr_name, _if_name, IFNAMSIZ);
//   }
//
//   /* try to create the device */
//   if( (_err = ioctl(_if_fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
//	   _if_fd = 0;
//   	   _if_error = errno;
//   	   setErrStr("ioctl(): ", strerror(errno));
//   	   close(_if_fd);
//   	   return 0;
//   }
//
//  /* if the operation was successful, write back the name of the
//   * interface to the variable "dev", so the caller can know
//   * it. Note that the caller MUST reserve space in *dev (see calling
//   * code below) */
//  strcpy(_if_name, ifr.ifr_name);
//
//  /* this is the special file descriptor that the caller will use to talk
//   * with the virtual interface */
//  return _if_fd;
//}

Nan::Persistent<Function> PseudoFs::constructor;

//Persistent<ObjectTemplate> PseudoFs::prototype;

NAN_METHOD(PseudoFs::Init) {
}


//void PseudoFs::ExtendFrom(const Arguments& args) {
//	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
//	tpl->SetClassName(String::NewSymbol("PseudoFs"));
//	tpl->InstanceTemplate()->SetInternalFieldCount(1);
//
//	tpl->PrototypeTemplate()->SetInternalFieldCount(2);
//
//	if(args.Length() > 0) {
//		if(args[0]->IsObject()) {
//			Local<Object> base = args[0]->ToObject();
//			Local<Array> keys = base->GetPropertyNames();
//			for(int n=0;n<keys->Length();n++) {
//				Local<String> keyname = keys->Get(n)->ToString();
//				tpl->InstanceTemplate()->Set(keyname, base->Get(keyname));
//			}
//		}
//	}
//
//
//	tpl->InstanceTemplate()->Set(String::NewSymbol("isCreated"), FunctionTemplate::New(IsCreated)->GetFunction());
//	tpl->InstanceTemplate()->Set(String::NewSymbol("create"), FunctionTemplate::New(Create)->GetFunction());
//	tpl->InstanceTemplate()->SetAccessor(String::New("ifname"), GetIfName, SetIfName);
//	tpl->InstanceTemplate()->SetAccessor(String::New("fd"), GetIfFD, SetIfFD);
//	tpl->InstanceTemplate()->SetAccessor(String::New("flags"), GetIfFlags, SetIfFlags);
//	tpl->InstanceTemplate()->SetAccessor(String::New("lastError"), GetLastError, SetLastError);
//	tpl->InstanceTemplate()->SetAccessor(String::New("lastErrorStr"), GetLastErrorStr, SetLastErrorStr);
//
//	tpl->InstanceTemplate()->SetAccessor(String::New("_readChunkSize"), GetReadChunkSize, SetReadChunkSize);
//	tpl->InstanceTemplate()->Set(String::NewSymbol("_open"), FunctionTemplate::New(Open)->GetFunction());
//	tpl->InstanceTemplate()->Set(String::NewSymbol("_close"), FunctionTemplate::New(Close)->GetFunction());
//	tpl->InstanceTemplate()->Set(String::NewSymbol("_getData"), FunctionTemplate::New(GetData)->GetFunction());
//	tpl->InstanceTemplate()->Set(String::NewSymbol("_sendData"), FunctionTemplate::New(SendData)->GetFunction());
//
//
////	PseudoFs::prototype = Persistent<ObjectTemplate>::New(tpl->PrototypeTemplate());
//	PseudoFs::constructor = Persistent<Function>::New(tpl->GetFunction());
//
//}


/** PseudoFs(opts)
 * opts {
 * 	    ifname: "tun77"
 * }
 * @param args
 * @return
 **/

NAN_METHOD(PseudoFs::New) {
	PseudoFs* obj = NULL;

	if (info.IsConstructCall()) {
	    // Invoked as constructor: `new MyObject(...)`
//	    double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
		if(info.Length() > 0) {
			if(!info[0]->IsString()) {
				Nan::ThrowTypeError("Improper first arg to ProcFs cstor. Must be a string.");
				return;
			}

			Nan::Utf8String v8str(info[0]->ToString());
			obj = new PseudoFs(v8str.operator *());

		} else {
			Nan::ThrowTypeError("First arg must be a string path.");
			return;
		}

		obj->Wrap(info.This());
	    info.GetReturnValue().Set(info.This());
	} else {
	    // Invoked as plain function `MyObject(...)`, turn into construct call.
	    const int argc = 1;
	    Local<Value> argv[argc] = { info[0] };
	    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
	    info.GetReturnValue().Set(cons->NewInstance(argc,argv));
	}

}
//
//Handle<Value> ProcFs::NewInstance(const Arguments& args) {
//	HandleScope scope;
//	int n = args.Length();
//	Local<Object> instance;
//
//	if(args.Length() > 0) {
//		Handle<Value> argv[n];
//		for(int x=0;x<n;x++)
//			argv[n] = args[n];
//		instance = ProcFs::constructor->NewInstance(n, argv);
//	} else {
//		instance = ProcFs::constructor->NewInstance();
//	}
//
//	return scope.Close(instance);
//}
//
//
//void ProcFs::SetIfName(Local<String> property, Local<Value> val, const AccessorInfo &info) {
//	HandleScope scope;
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(info.This());
//
//	if(val->IsString()) {
//		v8::String::Utf8Value v8str(val);
//		obj->setIfName(v8str.operator *(),v8str.length());
//	} else {
//		ERROR_OUT( "Invalid assignment to ProcFs object->ifname\n");
//	}
////	obj->SetIfName()
//}
//
//Handle<Value> ProcFs::GetIfName(Local<String> property, const AccessorInfo &info) {
//	HandleScope scope;
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(info.This());
//	if(obj->_if_name)
//		return scope.Close(String::New(obj->_if_name, strlen(obj->_if_name)));
//	else
//		return scope.Close(Undefined());
//}
//
//void ProcFs::SetIfFD(Local<String> property, Local<Value> val, const AccessorInfo &info) {
//	// does nothing - read only
//}
//
//Handle<Value> ProcFs::GetIfFD(Local<String> property, const AccessorInfo &info) {
//	HandleScope scope;
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(info.This());
//	if(obj->_if_fd) // 0 is default which is nothing (no device created)
//		return scope.Close(Integer::New(obj->_if_fd));
//	else
//		return scope.Close(Undefined());
//}

//void PseudoFs::SetReadChunkSize(Local<String> property, Local<Value> val, const AccessorInfo &info) {
void PseudoFs::SetReadChunkSize(v8::Local<v8::String> property,
	                            v8::Local<v8::Value> val,
	                            const Nan::PropertyCallbackInfo<v8::Value>& info) {
	PseudoFs* obj = ObjectWrap::Unwrap<PseudoFs>(info.This());
	if(val->IsInt32()) {
		obj->read_chunk_size = (int) val->Int32Value();
	} else {
		ERROR_OUT("Assignment to ->read_chunk_size with non Int32 type.");
	}

}

void PseudoFs::GetReadChunkSize(v8::Local<v8::String> property,
                            const Nan::PropertyCallbackInfo<v8::Value>& info) {
	PseudoFs* obj = ObjectWrap::Unwrap<PseudoFs>(info.This());
	info.GetReturnValue().Set(Nan::New((uint32_t) obj->read_chunk_size));
}


//void ProcFs::SetLastError(Local<String> property, Local<Value> val, const AccessorInfo &info) {
//	// does nothing - read only
//}
//
//Handle<Value> ProcFs::GetLastError(Local<String> property, const AccessorInfo &info) {
//	HandleScope scope;
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(info.This());
//	return scope.Close(Integer::New(obj->_if_error));
//}
//
//void ProcFs::SetLastErrorStr(Local<String> property, Local<Value> val, const AccessorInfo &info) {
//	// does nothing - read only
//}
//
//Handle<Value> ProcFs::GetLastErrorStr(Local<String> property, const AccessorInfo &info) {
//	HandleScope scope;
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(info.This());
//	if(obj->_err_str)
//		return scope.Close(String::New(obj->_err_str, strlen(obj->_err_str)));
//	else
//		return scope.Close(Undefined());
//}
//
//void ProcFs::SetIfFlags(Local<String> property, Local<Value> val, const AccessorInfo &info) {
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(info.This());
//	if(val->IsInt32()) {
//		obj->_if_flags = (int) val->ToInt32()->Int32Value();
//	} else {
//		ERROR_OUT("Assignment to ->_if_flags with non Int32 type.");
//	}
//}
//
//Handle<Value> ProcFs::GetIfFlags(Local<String> property, const AccessorInfo &info) {
//	HandleScope scope;
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(info.This());
//	return scope.Close(Integer::New(obj->_if_flags));
//}
//
//
//Handle<Value> ProcFs::IsCreated(const Arguments &args) {
//	HandleScope scope;
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(args.This());
//	if(obj->_if_fd) // 0 is default which is nothing (no device created)
//		return scope.Close(Boolean::New(true));
//	else
//		return scope.Close(Boolean::New(false));
//}



//Handle<Value> ProcFs::GetData(const Arguments& args) {
//	HandleScope scope;
//	if(args.Length() > 0 && args[0]->IsFunction()) {
//		int sizereq = 0;
//		if(args.Length() > 1 && args[1]->IsInt32())
//			sizereq = (int) args[1]->Int32Value();
//		ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(args.This());
//
//		ProcFs::readReq *req = new ProcFs::readReq(obj);
//		if(sizereq < obj->read_chunk_size) sizereq = obj->read_chunk_size; // read at least the MTU, regardless of req read size
//		// FIXME for node 0.12 this will change. Take note.
//		Handle<Object> buf = UNI_BUFFER_NEW(sizereq);
//		// make new Buffer object. Make it Persistent to keep it around after the HandleScope closes.
//		// we will do the read in a different thread. We don't want to call v8 in another thread, so just do the unwrapping here before we do the work..
//		// in the work we will just copy stuff to the _backing store.
//		req->buffer = Persistent<Object>::New(buf);
//
////		buf->Ref();
//		req->_backing = node::Buffer::Data(buf);
//		req->len = sizereq;
//		req->completeCB = Persistent<Function>::New(Local<Function>::Cast(args[0]));
//		// queue up read job...
//		DBG_OUT("Queuing work for read()\n");
//		uv_queue_work(uv_default_loop(), &(req->work), ProcFs::do_read, ProcFs::post_read);
//
//		return scope.Close(Undefined());
//	} else {
//		return ThrowException(Exception::TypeError(String::New("send() -> Need at least two params: getData([int32], [function])")));
//	}
//}


void PseudoFs::uv_close_handle_cb(uv_handle_t* handle) {

}

#if (UV_VERSION_MAJOR > 0)
void PseudoFs::timercb_pseudofs(uv_timer_t* h) {
#else
void PseudoFs::timercb_pseudofs(uv_timer_t *h, int status) {
#endif
//	workReq *req = (workReq *) h->data;
	DBG_OUT("timeout on eventloop for req.");
	uv_timer_stop(h);
	uv_unref((uv_handle_t*) h);
	uv_close((uv_handle_t*) h, uv_close_handle_cb);
}
/**
 *
 * @param
 * fs.readFile(filename, [options], callback)
 * callback = function(error,Buffer,readlen) {}
 */
NAN_METHOD(PseudoFs::ReadPseudofile) {
	int cb_index = 1;
	if(info.Length() > 1 && info[0]->IsString()) {
		if((info.Length() > 2) ) {
			if(info[2]->IsFunction())
				cb_index = 2;
			else {
				Nan::ThrowTypeError("pseudofs -> Need at least two params: readPseudofile(filename, [options], callback)");
				return;
			}

		}
		if(info.Length() == 2) {
			if(info[1]->IsFunction())
				cb_index = 1;
			else {
				Nan::ThrowTypeError("pseudofs -> Need at least two params: readPseudofile(filename, [options], callback)");
				return;
			}

		}

		// FIXME - process options {} arg[1]

		Nan::Utf8String v8str(info[0]->ToString());
		PseudoFs *obj = new PseudoFs(v8str.operator *());
		workReq *req = new PseudoFs::workReq(obj,workType::OPEN|workType::READ|workType::CLOSE|workType::SSHOT); // open,read,close - single shot

#ifdef USE_UV_REF
		if(req->ref) {
			uv_timer_init(uv_default_loop(),&req->timeoutHandle);
		}
#endif
		// FIXME for node 0.12 this will change. Take note.
		// make new Buffer object. Make it Persistent to keep it around after the HandleScope closes.
		// we will do the read in a different thread. We don't want to call v8 in another thread, so just do the unwrapping here before we do the work..
		// in the work we will just copy stuff to the _backing store.
		//		Handle<Object> buf = UNI_BUFFER_NEW(READ_DEFAULT_CHUNK_SIZE);
		Nan::MaybeLocal<Object> buf = Nan::NewBuffer(READ_DEFAULT_CHUNK_SIZE);
		Local<Object> buf_actual;
		if(buf.ToLocal(&buf_actual)) {
			req->_backing = node::Buffer::Data(buf_actual);
			req->buffer.Reset(buf_actual);
			req->completeCB = new Nan::Callback(Local<Function>::Cast(info[cb_index]));
			// queue up read job...
			DBG_OUT("Queuing work for readPseudofile()\n");
			uv_queue_work(uv_default_loop(), &(req->work), PseudoFs::do_work, PseudoFs::post_work);

		} else {
			ERROR_OUT("Failure to create Buffer object.");
#ifdef USE_UV_REF
			if(req->ref) {
//				uv_timer_stop(&req->timeoutHandle);
				uv_unref((uv_handle_t *)&req->timeoutHandle);
			}
#endif
			// TODO call callback as Failure
		}
//		req->buffer = Persistent<Object>::New(buf);
//		req->_backing = node::Buffer::Data(buf);
//		if(req->ref) {
//			node::Buffer *b = node::Buffer::Unwrap(buf);
//			b->Ref();
//		}
//		req->completeCB = Persistent<Function>::New(Local<Function>::Cast(info[cb_index]));

//		uv_ref();
	} else {
		Nan::ThrowTypeError("pseudofs -> Need at least two params: readPseudofile(filename, [options], callback)");
	}
}

void PseudoFs::do_work(uv_work_t *req) {
	workReq *job = (workReq *) req->data;
	DBG_OUT("do_work()\n");
	job->_errno = 0;

//	if(job->ref) {
////		uv_timer_init(uv_default_loop(),&job->timeoutHandle);
////		job->timeoutHandle.data = job;
////		uv_timer_start(&job->timeoutHandle,timercb_pseudofs,job->timeout*job->retries,0);
//		uv_ref((uv_handle_t *)&job->timeoutHandle);
//	}

	if(job->t & workType::OPEN) {
		if(job->self->_path) {
			DBG_OUT("doing OPEN: %s\n", job->self->_path);
			job->_fs_flags |= job->self->_fs_flags;
			if((job->t & workType::READ) && !(job->t & workType::WRITE))
				job->_fs_flags |= O_RDONLY;
			else if((job->t & workType::WRITE) && !(job->t & workType::READ))
				job->_fs_flags |= O_WRONLY;
			else { // if OPEN job only, then put in default flags if none stated
				if(!job->_fs_flags)
					job->_fs_flags = O_RDWR;
			}

			job->_fd = open(job->self->_path, job->_fs_flags);

			if(job->_fd == -1) {
				job->_errno = errno;
				DBG_OUT("got error %d\n",job->_errno);
				return;
			} else {
				job->self->_fs_flags = job->_fs_flags; // update flags used in PseudoFs object
			}
		}
	}


	if(!job->_errno && (job->t & workType::READ)) {
		int numBytes = job->_reqSize;
		bool eof = false;
		chunk *nextChunk = NULL;
		if(!numBytes) numBytes = READ_DEFAULT_CHUNK_SIZE;
		int total = 0;
		int r = 0;
		while(!eof) {
			if(total < numBytes) { // use the backing buffer for as long as we can... (most pseduo fs use cases)
				DBG_OUT("doing read() [total=%d]\n",total);
				r = read(job->_fd,job->_backing+total,numBytes);
			} else {               // otherwise, use auxiliary buffers. slower.
				if(nextChunk) {
					nextChunk->_next = new chunk(READ_DEFAULT_CHUNK_SIZE);
					nextChunk = nextChunk->_next;
				} else {
					job->_extras = new chunk(READ_DEFAULT_CHUNK_SIZE);
					nextChunk = job->_extras;
				}
				r = read(job->_fd,nextChunk->_buf,numBytes);
				nextChunk->_len = r;
			}
			if(r == -1) {
				job->_errno = errno;
				break;
			} else {
				total += r;
			}
			if(r==0 || (job->t & workType::SSHOT)) eof = true; // if no more data, or its a single-shot, then done.
		}
		DBG_OUT("total read: %d\n",total);
		job->len = total;
	}

	if(!job->_errno && (job->t & workType::WRITE)) {
		int numBytes = job->len;
		bool eof = false;
		chunk *nextChunk = NULL;
		int total = 0;
		int r = 0;
		int retry = 0;
		DBG_OUT("doing write() loop: [%d] %s \n",job->len, job->_backing);
		while(job->len && (job->len > total)) {
			DBG_OUT("doing write()\n");
			r = write(job->_fd,job->_backing+total,numBytes);
			if(r == -1) {
				job->_errno = errno;
				break;
			} else {
				total += r;
			}
			if(r == 0) { // if no error, but no bytes written
				retry++;
				if(retry > job->retries) {
					job->_errno = PSEUDO_PARTIAL_WRITE;
					break;
				} else
					usleep(job->timeout*1000); // timeout to try to write() again - bad behavior on a normal FS, but useful for pseudo FS
			}
		}
		IF_DBG( if(total >= r) DBG_OUT("write complete.\n"); );
	}

	if((job->t & workType::CLOSE) && job->_fd) {  // attempt to close even if previous error
		int r = close(job->_fd);
		if(r==-1 && !job->_errno) {
			job->_errno = errno;
		}
	}

//	if(job->ref) {
////		uv_timer_stop(&job->timeoutHandle);
//		uv_unref((uv_handle_t *)&job->timeoutHandle);
//	}


//	if(job->_path) {
//		// open file, read all of it, close it. place in Buffer
//
//		int ret = read(job->self->_fd,job->_backing,job->len);
//		DBG_OUT("ret = %d\n", ret);
//		if(ret < 0) {
//			job->_errno = errno;  // an error occurred, so record error info
//			job->len = 0;
//		} else {
//			job->len = ret; // record number of bytes read
//		}
//	}
}

void PseudoFs::post_work(uv_work_t *req, int status) {
	workReq *job = (workReq *) req->data;
	const unsigned argc = 3;
	Local<Value> argv[argc];
	PseudoFs *hiddenobj = NULL;

#ifdef USE_UV_REF
	if(job->ref) {
		uv_unref((uv_handle_t *)&job->timeoutHandle);
	}
#endif

	if(job->t & workType::SSHOT) {
		if(job->t & workType::READ) {
			if(job->_errno == 0) {
		//		Buffer* rawbuffer = ObjectWrap<Buffer>(job->buffer);
				if(job->completeCB) {
					DBG_OUT("SuccessCB (read)\n");
					argv[0] = Nan::Null();
					argv[1] = Nan::New(job->buffer);
					argv[2] = Nan::New(job->len);
					job->completeCB->Call(Nan::GetCurrentContext()->Global(),3,argv);
				}
			} else { // failure
				if(job->completeCB) {
					argv[0] = _errcmn::errno_to_JS(job->_errno,"Error in readPseudofile(): ");
					job->completeCB->Call(Nan::GetCurrentContext()->Global(),1,argv);
				}
			}
		}
		if(job->t & workType::WRITE) {
			if(job->_errno == 0) {
		//		Buffer* rawbuffer = ObjectWrap<Buffer>(job->buffer);
				if(job->completeCB) {
					argv[0] = Nan::Null();  // changed to null to match node.js behavior
					DBG_OUT("SuccessCB (write)\n");
					job->completeCB->Call(Nan::GetCurrentContext()->Global(),1,argv);
				}
			} else { // failure
				if(job->completeCB) {
					DBG_OUT("SuccessCB (write, failure)\n");
					argv[0] = _errcmn::errno_to_JS(job->_errno,"Error in writePseudofile(): ");
					job->completeCB->Call(Nan::GetCurrentContext()->Global(),1,argv);
				}
			}
		}


		if(job->self) {
			hiddenobj = job->self;
		}
	} else {
		DBG_OUT("!!! not implemented yet !!!");
	}


//	if(job->buffer->IsUndefined()) {
//		ERROR_OUT("**** Failure on read: Why is buffer not defined??\n");
//	} else
//		argv[0] = job->buffer->ToObject();
//	argv[1] = Integer::New(job->len);
//
//	if(job->_errno == 0) {
////		Buffer* rawbuffer = ObjectWrap<Buffer>(job->buffer);
//
//		if(!job->completeCB->IsUndefined()) {
//			job->completeCB->Call(Context::GetCurrent()->Global(),2,argv);
//		}
//	} else { // failure
//		if(!job->completeCB->IsUndefined()) {
//			argv[2] = _pseudo_fs::errno_to_JS(job->_errno,"Error in read(): ");
//			job->completeCB->Call(Context::GetCurrent()->Global(),3,argv);
//		}
//	}
//
	if(!job->buffer.IsEmpty()) job->buffer.Reset();
	delete job; // should delete Persistent Handles and allow the Buffer object to be GC'ed
	if(hiddenobj) delete hiddenobj;
}

//void PseudoFs::do_read(uv_work_t *req) {
//	readReq *job = (readReq *) req->data;
//	DBG_OUT("do_read()\n");
//
//	if(job->_path) {
//		// open file, read all of it, close it. place in Buffer
//
//		int ret = read(job->self->_fd,job->_backing,job->len);
//		DBG_OUT("ret = %d\n", ret);
//		if(ret < 0) {
//			job->_errno = errno;  // an error occurred, so record error info
//			job->len = 0;
//		} else {
//			job->len = ret; // record number of bytes read
//		}
//	}
//}
//
//void PseudoFs::post_read(uv_work_t *req, int status) {
//	readReq *job = (readReq *) req->data;
//
//	const unsigned argc = 3;
//	Local<Value> argv[argc];
//	if(job->buffer->IsUndefined()) {
//		ERROR_OUT("**** Failure on read: Why is buffer not defined??\n");
//	} else
//		argv[0] = job->buffer->ToObject();
//	argv[1] = Integer::New(job->len);
//
//	if(job->_errno == 0) {
////		Buffer* rawbuffer = ObjectWrap<Buffer>(job->buffer);
//
//		if(!job->completeCB->IsUndefined()) {
//			job->completeCB->Call(Context::GetCurrent()->Global(),2,argv);
//		}
//	} else { // failure
//		if(!job->completeCB->IsUndefined()) {
//			argv[2] = _pseudo_fs::errno_to_JS(job->_errno,"Error in read(): ");
//			job->completeCB->Call(Context::GetCurrent()->Global(),3,argv);
//		}
//	}
//
//
//	delete job; // should delete Persistent Handles and allow the Buffer object to be GC'ed
//}

/**
 * Writes data to Pseudo file system (procfs, sysfs, devfs)
 * callback = function(error) {}
 * pseudoFS.writeFile(filename, data, [options], callback)
 */
NAN_METHOD(PseudoFs::WritePseudofile) {
	int opts_in = 0;
	if(info.Length() < 3) {
		return Nan::ThrowTypeError("pseudofs -> Need at least two params: writePseudo(filename, data, [options], callback)");
	}

	if(info.Length() == 3) {
		opts_in = 0;
	}
	if(!info[2+opts_in]->IsFunction())
		return Nan::ThrowTypeError("pseudofs -> Need at least two params: writePseudo(filename, data, [options], callback)");

	Nan::Utf8String v8str(info[0]);
	PseudoFs *obj = new PseudoFs(v8str.operator *());
	workReq *req = new PseudoFs::workReq(obj,workType::OPEN|workType::WRITE|workType::CLOSE|workType::SSHOT); // open,read,close - single shot


	if(((info[1]->IsObject() && Buffer::HasInstance(info[1])) || info[1]->IsString()) && info[0]->IsString()) {
		if(info[1]->IsString()) {
			Nan::Utf8String v8dat(info[1]);
			req->_backing = strdup(v8dat.operator *()); // copy the string to the request
			req->freeBacking = true; // mark to free() on delete
			req->len = strlen(req->_backing);
		} else {
			req->buffer.Reset(info[1]->ToObject()); // keep the Buffer persistent until the write is done... (will be removed when req is deleted)
			req->_backing = node::Buffer::Data(info[1]->ToObject());
			req->len = node::Buffer::Length(info[1]->ToObject());
		}

		// TODO process options

//		req->completeCB = Persistent<Function>::New(Local<Function>::Cast(args[opts_in+2]));
		req->completeCB = new Nan::Callback(Local<Function>::Cast(info[opts_in+2]));
		// queue up write job...
		DBG_OUT("Queuing work for writePseudofile()\n");
		uv_queue_work(uv_default_loop(), &(req->work), PseudoFs::do_work, PseudoFs::post_work);

		return;
	} else {
		return Nan::ThrowTypeError("pseudofs -> Need at least two params: writePseudo(filename, data {string|Buffer}, [options], callback)");
	}
}
//
//void PseudoFs::do_write(uv_work_t *req) {
//	writeReq *job = (writeReq *) req->data;
//
//	int ret = 0;
//	int written = 0;
//	char *buf = job->_backing;
//	job->_errno = 0;
//	if(job->_path) {
//
//		while (ret >= 0 && written < job->len) {
//			int ret = write(job->self->_fd,buf,job->len - written);
//			if(ret < 0) {
//				job->_errno = errno;  // an error occurred, so record error info
//				break;
//			} else {
//				written += ret; // record number of bytes written
//			}
//			buf += written;
//		}
//		job->len = written;
//	}
//	// TODO do read
//}
//
//void PseudoFs::post_write(uv_work_t *req, int status) {
//	writeReq *job = (writeReq *) req->data;
//
//	const unsigned argc = 2;
//	Local<Value> argv[argc];
//	argv[0] = Integer::New(job->len); // first param to call back is always amount of bytes written
//
//	if(job->_errno == 0) {
////		Buffer* rawbuffer = ObjectWrap<Buffer>(job->buffer);
//		if(!job->onSendSuccessCB->IsUndefined()) {
//			job->onSendSuccessCB->Call(Context::GetCurrent()->Global(),1,argv);
//		}
//	} else { // failure
//		if(!job->onSendFailureCB->IsUndefined()) {
//			argv[1] = _pseudo_fs::errno_to_JS(job->_errno,"Error in write(): ");
//			job->onSendFailureCB->Call(Context::GetCurrent()->Global(),2,argv);
//		}
//	}
//
//	delete job;
//}


// NOT IMPLEMENTED
//Handle<Value> PseudoFs::Open(const Arguments& args) {
//	HandleScope scope;
//	PseudoFs* obj = ObjectWrap::Unwrap<PseudoFs>(args.This());
//
//	// FIXME - this only uses the fd created by Create() - we later should try to reopen a closed TUN device.
//
//	if(obj->_if_fd > 0) {
//		return scope.Close(Boolean::New(true));
//	} else
//		return scope.Close(Boolean::New(false));
//
//}

/*
 * NOT IMPLEMENTED
Handle<Value> PseudoFs::Close(const Arguments& args) {
	HandleScope scope;
	PseudoFs* obj = ObjectWrap::Unwrap<PseudoFs>(args.This());

	if(obj->_fd > 0) {
		if(close(obj->_fd) < 0) {
			obj->_fs_error = errno;  // an error occurred, so record error info
			obj->setErrStr("ioctl(): ", strerror(errno));
			// FIXME - return object
			return scope.Close(Boolean::New(false));
		} else
			return scope.Close(Boolean::New(true));
	} else {
		obj->setErrStr("not open!","");
		return scope.Close(Boolean::New(false));
	}
}
*/


/**
 * Creates the TUN interface.
 */
//Handle<Value> PseudoFs::Create(const Arguments& args) {
//	HandleScope scope;
//	PseudoFs* obj = ObjectWrap::Unwrap<PseudoFs>(args.This());
//
//	obj->_if_error = 0;
//	obj->tun_create();
//
//	if(!obj->_if_error)
//		return scope.Close(Boolean::New(true));
//	else {
//		return scope.Close(Boolean::New(false));
//	}
//}




void InitAll(Handle<Object> exports, Handle<Object> module) {
//	NodeTransactionWrapper::Init();
//	NodeClientWrapper::Init();
//	exports->Set(String::NewSymbol("cloneRepo"), FunctionTemplate::New(CreateClient)->GetFunction());


//	TunInterface::Init();
	Nan::Set(exports,Nan::New<String>("InitPseudoFS").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(PseudoFs::Init)->GetFunction());
	Nan::Set(exports,Nan::New<String>("psuedoFS").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(PseudoFs::New)->GetFunction());
	Nan::Set(exports,Nan::New<String>("readPseudo").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(PseudoFs::ReadPseudofile)->GetFunction());
	Nan::Set(exports,Nan::New<String>("writePseudo").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(PseudoFs::WritePseudofile)->GetFunction());

	Handle<Object> consts = Nan::New<Object>();
	_errcmn::DefineConstants(consts);
	Nan::Set(exports,Nan::New<String>("CONSTS").ToLocalChecked(), consts);


//	exports->Set(String::NewSymbol("read"), FunctionTemplate::New(Read)->GetFunction());
//	exports->Set(String::NewSymbol("write"), FunctionTemplate::New(Write)->GetFunction());
//	exports->Set(String::NewSymbol("close"), FunctionTemplate::New(Close)->GetFunction());

//	exports->Set(String::NewSymbol("assignRoute"), FunctionTemplate::New(AssignRoute)->GetFunction());
//	exports->Set(String::NewSymbol("setIfFlags"), FunctionTemplate::New(SetIfFlags)->GetFunction());
//	exports->Set(String::NewSymbol("unsetIfFlags"), FunctionTemplate::New(UnsetIfFlags)->GetFunction());

//	exports->Set(String::NewSymbol("_TunInterface_cstor"), TunInterface::constructor);

	//	exports->Set(String::NewSymbol("_TunInterface_proto"), TunInterface::prototype);

//	exports->Set(String::NewSymbol("shutdownTunInteface"), FunctionTemplate::New(ShutdownTunInterface)->GetFunction());

}

NODE_MODULE(pseudofs, InitAll)









///**
// * Bring the interface up
// */
//Handle<Value> ProcFs::IfUp(const Arguments& args) {
//	HandleScope scope;
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(args.This());
//
//	char if_tmp_buf[255];
//
//	char *errstr = NULL;
//
//
//	int err = 0;
//	obj->_if_error = 0;
//	obj->tun_create();
//
//	if(!obj->_if_error)
//		return scope.Close(Boolean::New(true));
//	else {
//		return scope.Close(Boolean::New(false));
//	}
//}
//
///**
// * Bring the interface down
// */
//Handle<Value> ProcFs::IfDown(const Arguments& args) {
//	HandleScope scope;
//	ProcFs* obj = ObjectWrap::Unwrap<ProcFs>(args.This());
//
//	char if_tmp_buf[255];
//
//	char *errstr = NULL;
//
//
//	int err = 0;
//	obj->_if_error = 0;
//	obj->tun_create();
//
//	if(!obj->_if_error)
//		return scope.Close(Boolean::New(true));
//	else {
//		return scope.Close(Boolean::New(false));
//	}
//}
//
