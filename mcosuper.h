#ifndef MCOSUPER_H
#define MCOSUPER_H

class McoSuper
{
private:
    McoSuper();
	
public:
    static  McoSuper* getObject();
	/*
	* 获取当前时间
	* return 毫秒
	*/
	static long long getNowTime();
};

#endif // MCOSUPER_H
