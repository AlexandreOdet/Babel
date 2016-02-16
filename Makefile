##
## Makefile for Makefile in /home/lopez_t/Babel
##
## Made by Thibaut Lopez
## Login   <lopez_t@epitech.net>
##
## Started on  Thu Oct 22 10:32:09 2015 Thibaut Lopez
## Last update Wed Nov  4 17:33:55 2015 Thibaut Lopez
##

MAKE=		make -C

SERVER=		server/babel

SERVER_BIN=	babel_server

SERVER_CONF=	server/.users.json

SERVER_CONF_L=	.users.json

SERVER_REPO=	server

CLIENT=		client/client

CLIENT_BIN=	babel_client

CLIENT_REPO=	client

all:		$(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN):
		$(MAKE) $(SERVER_REPO)
		@ln -s $(SERVER_CONF) $(SERVER_CONF_L)
		@ln -s $(SERVER) $(SERVER_BIN)

$(CLIENT_BIN):
		(cd client; qmake)
		$(MAKE) $(CLIENT_REPO)
		@ln -s $(CLIENT) $(CLIENT_BIN)

clean:
		$(MAKE) $(SERVER_REPO) clean
		$(MAKE) $(CLIENT_REPO) clean

fclean:
		@rm -f $(SERVER_BIN)
		@rm -f $(SERVER_CONF_L)
		$(MAKE) $(SERVER_REPO) fclean
		@rm -f $(CLIENT_BIN)
		$(MAKE) $(CLIENT_REPO) distclean

re:
		$(MAKE) $(SERVER_REPO) re
		$(MAKE) $(CLIENT_REPO) re
