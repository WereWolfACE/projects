#ifndef _DBGSERVER_H_
#define _DBGSERVER_H_

#include "LuaBind.h"

namespace Core {

class DbgServer
{
public:
	DbgServer();
	~DbgServer();

	void Init();
	void Update(float dt);
	
	void OnPacket(unsigned short packet_type, unsigned short data_size, unsigned char* p);
	void SendData(const char* s, int len);
	void SendString(const char* s);
	void SendReady();
	
private:
	class Impl;
	std::unique_ptr<Impl> _impl;
	
	DbgServer(const DbgServer&);
	DbgServer& operator=(const DbgServer&);
};

#if defined(_DEBUG)
extern DbgServer dbgServer;
#endif // _DEBUG

void DbgServerSend(const luabind::object& obj);

} // end of namespace

#endif //_DBGSERVER_H_
