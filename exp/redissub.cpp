#include <iostream>
#include <cstdlib>
#include <string>
#include "hiredis.h"

using namespace std;

int main()
{
	//1、连接Redis服务器
	redisContext *context = redisConnect("127.0.0.1", 6379);
	if (context->err)
	{
		std::cout << "can not connect to redisserver: 127.0.0.1,port:6379" << endl;
		std::cout << "reason:" << context->errstr << endl;
		redisFree(context);
		context = NULL;
		return 0;
	}

	//2、校验密码
	char auth[8] = "test123";
	redisReply *reply = (redisReply *)redisCommand(context, "AUTH %s", auth);
	if (NULL == reply)
	{
		std::cout << "wrong password!" << endl;
		freeReplyObject(reply);
		redisFree(context);
		context = NULL;
		return 0;
	}
	freeReplyObject(reply);

	//3、选择数据库 - 不选择也可
	reply = (redisReply *)redisCommand(context, "SELECT %d", 1);
	if (NULL == reply || REDIS_REPLY_ERROR == reply->type)
	{
		std::cout << "select failed!" << endl;
		freeReplyObject(reply);
		redisFree(context);
		context = NULL;
		return 0;
	}

	std::cout << "subscribe redischat!" << endl;

	//4、订阅消息  可以订阅多个消息 PSUBSCRIBE pattern 
	reply = (redisReply *)redisCommand(context, "SUBSCRIBE %s", "redischat");
	if (NULL == reply || reply->type != REDIS_REPLY_ARRAY)//订阅成功返回一个数组标识
	{
		std::cout << "subscribe failed!" << endl;
		freeReplyObject(reply);
		redisFree(context);
		context = NULL;
		return 0;
	}
	freeReplyObject(reply);

	//5、阻塞等待订阅消息
	while (true)
	{
		void *_reply = nullptr;
		if (redisGetReply(context, &_reply) != REDIS_OK)
		{
			continue;
		}
		reply = (redisReply*)_reply;
		for (int nIndex = 0; nIndex < reply->elements; nIndex++)
		{
			std::cout << nIndex + 1 << ")";
			std::cout << reply->element[nIndex]->str << std::endl;
		}
		freeReplyObject(reply);
		std::cout << "***************" << std::endl;
	}
	redisFree(context);
	context = NULL;
	return 0;
}