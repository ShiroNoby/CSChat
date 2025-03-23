cmake_minimum_required(VERSION 3.0.2)

project(CSChat)

option(Client "build client" ON )

if(Client)
	subdirs(Client)
else()
	subdirs(Server)
endif()