#pragma once
//#define SET_CLA_INST(name,val) inst->Set(String::New(#name),val)
//#define SET_CLA_INST_RO(name,val) inst->Set(String::New(#name),val,ReadOnly)
#define SET_CLA_FUNC(name) func->Set(String::New(#name),FunctionTemplate::New(&name)->GetFunction(),ReadOnly)
//设置常数, 值是前缀+名字对应的C++宏常数.
#define SET_CLA_CONST(name,pre) ft->Set(String::New(#name),Int32::New(pre##_##name),ReadOnly)
//设置v常数.
#define SET_CLA_CONST2(name,v) ft->Set(String::New(#name),Int32::New(v),ReadOnly)
#define SET_CLA_CONST3(name) ft->Set(String::New(#name),Int32::New(name),ReadOnly)
//设置类的访问器, 如果不需要get或set , 设置为 0 即可.
#define SET_CLA_ACCESSOR(name) func->SetAccessor(String::New(#name),_get,_set)
#define SET_CLA_SET_ACCESSOR(name,set) func->SetAccessor(String::New(#name),0,set)
#define SET_OBJ_ACCESSOR(obj,name) obj->SetAccessor(String::New(#name),_get,_set)
//
#define SET_PROP(obj,name,val) obj->Set(String::New(#name),val)
#define SET_PROP_RO(obj,name,val) obj->Set(String::New(#name),val,ReadOnly)
#define GET_PROP(obj,name) obj->Get(String::New(#name))
#define SET_VAR_TO_PROP(val,var,obj,name,T)	val = obj->Get(String::New(#name));if(!val->IsUndefined()){var = val->T##Value();}

#define SET_PROP2(obj,name,val,T) obj->Set(String::New(#name),T::New(val))
#define GET_PROP2(obj,name,val,T) val = obj->Get(String::New(#name))->T##Value()

#define SET_OBJ_CONST(name,pre) obj->Set(String::New(#name),Uint32::New(pre##_##name))
#define SET_OBJ_FUNC_RO(obj,name,func) obj->Set(String::New(#name),FunctionTemplate::New(&func)->GetFunction(),ReadOnly)
#define SET_OBJ_FUNC(obj,name,func) obj->Set(String::New(#name),FunctionTemplate::New(&func)->GetFunction())
#define SET_OF(name) obj->Set(String::New(#name),FunctionTemplate::New(&name)->GetFunction())

#define GET_ARGS_STR(i,str) if(args.Length()>i) GetString(args[i],str)
#define GET_ARGS_INT(i,def) (args.Length()>i?args[i]->Int32Value():def)
#define GET_ARGS_INT2(i,def,T) (args.Length()>i?(T)(UINT_PTR)args[i]->Int32Value():def)
#define GET_ARGS_VALUE(i,def,T) (args.Length()>i?args[i]->T##Value():def)
#define NEW_STR(s) String::New(#s)
#define NEW_WSTR(s) String::New((uint16_t*)s)

#define Bool(b) b?True():False()
#define RET(val,T)	return store.Close(T::New(val))
#define RET_INT(val) return store.Close(Int32::New(val))
#define RET_UINT(val) return store.Close(Uint32::New(val))