/*
A simple echo client.

It connects to a server, sends the specified string, and waits for the server to send it back.

Note that at the moment, czspas doesn't have synchronous sends/writes, so everything needs to be done
with the asyncXXX functions.
Synchronous sends/receives will probably be implemented at some point.

A completely synchronous client would be shorter than what's shown here.
*/

#include <crazygaze/spas/spas.h>

using namespace cz;
using namespace cz::spas;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(Service& service, const char* host, int port)
		: m_socket(service)
	{
		auto ec = m_socket.connect(host, port);
		if (ec)
			throw std::runtime_error(std::string("Error connecting to host: ") + ec.msg());
	}

	void start(const char* msg)
	{
		auto self(shared_from_this());
		auto len = strlen(msg);
		// Send the entire message. asyncSend does multiple sends if it needs to, to ensure it sends everything
		printf("Sending: %s\n", msg);
		asyncSend(m_socket, msg, len, -1, [this, self](const Error& ec, size_t transfered)
		{
			if (ec)
				throw std::runtime_error(ec.msg());
		});

		auto inBuf = std::shared_ptr<char>(new char[len+1], [](char* p) { delete[] p; });
		// Read up to "len" bytes. asyncReceive does multiple receives it if needs to, until it receives the specified
		// length, or an error happens.
		asyncReceive(m_socket, inBuf.get(), len, -1, [this, self, inBuf](const Error& ec, size_t transfered)
		{
			inBuf.get()[transfered] = 0;
			printf("Received: %s\n", inBuf.get());
			m_socket.getService().stop();
		});
	}

private:

	Socket m_socket;
};

int do_main(int argc, char* argv[])
{
	printf("EchoClient sample\n");
	if (argc != 3)
	{
		fprintf(stderr, "Usage: EchoClient <ip> <port>\n");
		return EXIT_FAILURE;
	}

	Service service;

	auto session = std::make_shared<Session>(service, argv[1], std::stoi(argv[2]));
	session->start("Hello World!");

	service.run();
	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	try
	{
		return do_main(argc, argv);
	}
	catch (std::exception& e)
	{
		fprintf(stderr, "Exception: %s\n", e.what());
		return EXIT_FAILURE;
	}
}

