##
## Makefile for Makefile in /home/lopez_t/Babel
##
## Made by Thibaut Lopez
## Login   <lopez_t@epitech.net>
##
## Started on  Mon Oct 19 13:56:13 2015 Thibaut Lopez
## Last update Sat Nov  7 23:39:58 2015 Thibaut Lopez
##

CXX=		g++ -W -Wall -Werror -std=c++11 -g3

RM=		rm -f

BOOSTFLAGS=	-lboost_system -lboost_date_time -lboost_thread

SRC_REPO=	src

API_REPO=	../API

INCLUDES=	-I./includes

API_INCLUDES=	-I$(API_REPO)/interface/ -I$(API_REPO)/includes

CXXFLAGS=	-W -Wall -Werror -std=c++11 $(INCLUDES) $(API_INCLUDES)

SRC=		$(SRC_REPO)/Server.cpp			\
		$(SRC_REPO)/UserInformation.cpp		\
		$(SRC_REPO)/Client.cpp			\
		$(SRC_REPO)/Channel.cpp			\
		$(SRC_REPO)/ChannelManager.cpp		\
		$(SRC_REPO)/PrivateManager.cpp		\
		$(SRC_REPO)/Response.cpp		\
		$(SRC_REPO)/Status.cpp			\
		$(SRC_REPO)/main.cpp

API_SRC=	$(API_REPO)/SNetwork/BoostNetwork.cpp	\
		$(API_REPO)/UserDB/UserDB.cpp

NAME=		babel

API_OBJ=	$(API_SRC:.cpp=.o)

OBJ=		$(SRC:.cpp=.o)

all:		$(NAME)

$(NAME):	$(API_OBJ) $(OBJ)
		$(CXX) $(API_OBJ) $(OBJ) -o $(NAME) $(INCLUDES) $(BOOSTFLAGS) $(API_INCLUDES)

clean:
		$(RM) $(API_OBJ)
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
