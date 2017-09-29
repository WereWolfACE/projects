#pragma once

//---------------------------------------------------------------------------
struct IGCRef;

//---------------------------------------------------------------------------
extern int gc_managed;

void gcAddRoot(IGCRef* gcRef);
void gcRemoveRoot(IGCRef* gcRef);
void gcTrack(IGCRef* gcRef);
void gcGreyObject(IGCRef* gcRef);
void gcSweep();
void gcPushStackRef(IGCRef* gcRef);
void gcPopGCRef();
void gcShutdown();

struct GCStackRef
{
	explicit GCStackRef(IGCRef* _ref)
	{
		gcPushStackRef(_ref);
	}

	~GCStackRef()
	{
		gcPopGCRef();
	}
};

#define GC_BLACKEN_DECL() void __gcBlacken()
#define GC_BLACKEN_DEF(A) void A::__gcBlacken()//определяет метод 'затенения' для объекта класса
#define GC_BLACKEN_CALL(A) A::__gcBlacken()//определяет метод 'затенения' для объекта класса

#define GC_SHOULD_TRACK(object) gcTrack(object); //вызывается, когда объект должен отслеживаться сборщиком мусора
#define GC_SHOULD_MARK(object) gcGreyObject(object);//вызывается, когда объект должен быть помечен
#define GC_ADD_REF(object) gcAddRoot(object);
#define GC_REMOVE_REF(object) gcRemoveRoot(object);
#define GC_PERFORM() gcSweep();

#define GC_STACKREF(object) GCStackRef ref##__FILE__##__LINE__(object);
#define GC_FINALIZER(ref) void gcFinalize(IGCRef* ref)
#define GC_UNMANAGED(operation) {gc_managed++; operation; gc_managed--;}