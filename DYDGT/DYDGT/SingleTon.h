#pragma once

template<class T>
class Singleton
{
private:
	static T* instance;
public:
	inline static T* getInstance(){
		if (instance == NULL){
			instance = new T;
		}
		return instance;
	}

	inline static void delInstance(){
		if (instance){
			delete(instance);
		}
	}

};

template<class T>
T* Singleton<T>::instance = NULL;

