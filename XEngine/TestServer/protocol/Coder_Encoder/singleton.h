#ifndef _Singleton_H__
#define _Singleton_H__


#define DECLARE_SINGLETON(cls)\
private:								\
	static cls*	sm_poSingleton;			\
										\
public:									\
	static void CreateInstance()		\
	{									\
		if(sm_poSingleton == NULL)		\
		{								\
			sm_poSingleton = new cls(); \
		}								\
	}									\
	static void DestroyInstance()		\
	{									\
		if(sm_poSingleton != NULL)		\
		{								\
			delete sm_poSingleton;		\
			sm_poSingleton = NULL;		\
		}								\
	}									\
	static cls* Instance()				\
	{									\
		return sm_poSingleton;			\
	}


#define INSTANCE_SINGLETON(type)	type* type::sm_poSingleton = NULL;
	

#endif