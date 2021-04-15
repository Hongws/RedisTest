#include <iostream>
#include <cstdlib>
#include <string>
#include "hiredis.h"

using namespace std;

int main()
{
	//1、连接服务器
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
		redisFree(context);
		context = NULL;
		return 0;
	}
	freeReplyObject(reply);

	//3、选择数据库
	reply = (redisReply *)redisCommand(context, "SELECT %d", 1);
	if (NULL == reply || REDIS_REPLY_ERROR == reply->type)
	{
		std::cout << "select failed!" << endl;
		redisFree(context);
		context = NULL;
		return 0;
	}
	std::cout << "publish redischat!" << endl;

	//4、推送消息
	while (true)
	{
		std::string Message;
		char s[1024];
		memset(s, 0, 1024);
		cin >> s;
		Message.append(s);
		reply = (redisReply *)redisCommand(context, "publish %s %s", "redischat", Message.c_str());
		if (NULL == reply || reply->type != REDIS_REPLY_INTEGER)//成功推送几个就会有几个 integer
		{
			break;
		}
		freeReplyObject(reply);
	}
	if (reply != NULL)freeReplyObject(reply);
	redisFree(context);
	context = NULL;
	return 0;
}
