#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/helper/HTTP_Client.o \
	${OBJECTDIR}/helper/crypto/sha1.o \
	${OBJECTDIR}/helper/crypto/tomcrypt/algorithms/crypt_argchk.o \
	${OBJECTDIR}/helper/crypto/tomcrypt/algorithms/md5.o \
	${OBJECTDIR}/helper/crypto/tomcrypt/algorithms/sha1.o \
	${OBJECTDIR}/helper/mongoose_http_server/MongooseDaemon.o \
	${OBJECTDIR}/helper/mongoose_http_server/callbacks/callbacks.o \
	${OBJECTDIR}/helper/mongoose_http_server/callbacks/chord_callbacks.o \
	${OBJECTDIR}/helper/mongoose_http_server/mongoose.o \
	${OBJECTDIR}/helper/request/Request.o \
	${OBJECTDIR}/helper/thread/Thread.o \
	${OBJECTDIR}/p2p/node/Node.o \
	${OBJECTDIR}/p2p/protocols/ProtocolSingleton.o \
	${OBJECTDIR}/p2p/protocols/ca/CertificationAuthority.o \
	${OBJECTDIR}/p2p/protocols/chord/AbstractChord.o \
	${OBJECTDIR}/p2p/protocols/chord/ChordNode.o \
	${OBJECTDIR}/p2p/protocols/chord/Query.o \
	${OBJECTDIR}/p2p/transport/http/TransportHTTP.o \
	${OBJECTDIR}/p2p/transport/http/http_operations.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libanonymouschordlibrary.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libanonymouschordlibrary.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libanonymouschordlibrary.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libanonymouschordlibrary.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libanonymouschordlibrary.a

${OBJECTDIR}/helper/HTTP_Client.o: helper/HTTP_Client.cpp
	${MKDIR} -p ${OBJECTDIR}/helper
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/HTTP_Client.o helper/HTTP_Client.cpp

${OBJECTDIR}/helper/crypto/sha1.o: helper/crypto/sha1.cpp
	${MKDIR} -p ${OBJECTDIR}/helper/crypto
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/crypto/sha1.o helper/crypto/sha1.cpp

${OBJECTDIR}/helper/crypto/tomcrypt/algorithms/crypt_argchk.o: helper/crypto/tomcrypt/algorithms/crypt_argchk.c
	${MKDIR} -p ${OBJECTDIR}/helper/crypto/tomcrypt/algorithms
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/crypto/tomcrypt/algorithms/crypt_argchk.o helper/crypto/tomcrypt/algorithms/crypt_argchk.c

${OBJECTDIR}/helper/crypto/tomcrypt/algorithms/md5.o: helper/crypto/tomcrypt/algorithms/md5.c
	${MKDIR} -p ${OBJECTDIR}/helper/crypto/tomcrypt/algorithms
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/crypto/tomcrypt/algorithms/md5.o helper/crypto/tomcrypt/algorithms/md5.c

${OBJECTDIR}/helper/crypto/tomcrypt/algorithms/sha1.o: helper/crypto/tomcrypt/algorithms/sha1.c
	${MKDIR} -p ${OBJECTDIR}/helper/crypto/tomcrypt/algorithms
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/crypto/tomcrypt/algorithms/sha1.o helper/crypto/tomcrypt/algorithms/sha1.c

${OBJECTDIR}/helper/mongoose_http_server/MongooseDaemon.o: helper/mongoose_http_server/MongooseDaemon.mm
	${MKDIR} -p ${OBJECTDIR}/helper/mongoose_http_server
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/mongoose_http_server/MongooseDaemon.o helper/mongoose_http_server/MongooseDaemon.mm

${OBJECTDIR}/helper/mongoose_http_server/callbacks/callbacks.o: helper/mongoose_http_server/callbacks/callbacks.cpp
	${MKDIR} -p ${OBJECTDIR}/helper/mongoose_http_server/callbacks
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/mongoose_http_server/callbacks/callbacks.o helper/mongoose_http_server/callbacks/callbacks.cpp

${OBJECTDIR}/helper/mongoose_http_server/callbacks/chord_callbacks.o: helper/mongoose_http_server/callbacks/chord_callbacks.cpp
	${MKDIR} -p ${OBJECTDIR}/helper/mongoose_http_server/callbacks
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/mongoose_http_server/callbacks/chord_callbacks.o helper/mongoose_http_server/callbacks/chord_callbacks.cpp

${OBJECTDIR}/helper/mongoose_http_server/mongoose.o: helper/mongoose_http_server/mongoose.cpp
	${MKDIR} -p ${OBJECTDIR}/helper/mongoose_http_server
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/mongoose_http_server/mongoose.o helper/mongoose_http_server/mongoose.cpp

${OBJECTDIR}/helper/request/Request.o: helper/request/Request.cpp
	${MKDIR} -p ${OBJECTDIR}/helper/request
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/request/Request.o helper/request/Request.cpp

${OBJECTDIR}/helper/thread/Thread.o: helper/thread/Thread.cpp
	${MKDIR} -p ${OBJECTDIR}/helper/thread
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/helper/thread/Thread.o helper/thread/Thread.cpp

${OBJECTDIR}/p2p/node/Node.o: p2p/node/Node.cpp
	${MKDIR} -p ${OBJECTDIR}/p2p/node
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/p2p/node/Node.o p2p/node/Node.cpp

${OBJECTDIR}/p2p/protocols/ProtocolSingleton.o: p2p/protocols/ProtocolSingleton.cpp
	${MKDIR} -p ${OBJECTDIR}/p2p/protocols
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/p2p/protocols/ProtocolSingleton.o p2p/protocols/ProtocolSingleton.cpp

${OBJECTDIR}/p2p/protocols/ca/CertificationAuthority.o: p2p/protocols/ca/CertificationAuthority.cpp
	${MKDIR} -p ${OBJECTDIR}/p2p/protocols/ca
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/p2p/protocols/ca/CertificationAuthority.o p2p/protocols/ca/CertificationAuthority.cpp

${OBJECTDIR}/p2p/protocols/chord/AbstractChord.o: p2p/protocols/chord/AbstractChord.cpp
	${MKDIR} -p ${OBJECTDIR}/p2p/protocols/chord
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/p2p/protocols/chord/AbstractChord.o p2p/protocols/chord/AbstractChord.cpp

${OBJECTDIR}/p2p/protocols/chord/ChordNode.o: p2p/protocols/chord/ChordNode.cpp
	${MKDIR} -p ${OBJECTDIR}/p2p/protocols/chord
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/p2p/protocols/chord/ChordNode.o p2p/protocols/chord/ChordNode.cpp

${OBJECTDIR}/p2p/protocols/chord/Query.o: p2p/protocols/chord/Query.cpp
	${MKDIR} -p ${OBJECTDIR}/p2p/protocols/chord
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/p2p/protocols/chord/Query.o p2p/protocols/chord/Query.cpp

${OBJECTDIR}/p2p/transport/http/TransportHTTP.o: p2p/transport/http/TransportHTTP.cpp
	${MKDIR} -p ${OBJECTDIR}/p2p/transport/http
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/p2p/transport/http/TransportHTTP.o p2p/transport/http/TransportHTTP.cpp

${OBJECTDIR}/p2p/transport/http/http_operations.o: p2p/transport/http/http_operations.cpp
	${MKDIR} -p ${OBJECTDIR}/p2p/transport/http
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/p2p/transport/http/http_operations.o p2p/transport/http/http_operations.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
