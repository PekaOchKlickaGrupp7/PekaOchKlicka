#pragma once

#pragma region Macros
#define MIN(X, Y) ((X)<(Y)?(X):(Y))
#define MAX(X, Y) ((X)>(Y)?(X):(Y))

#define SWAP(X, Y) \
(X)^=(Y);(Y)^=(X);(X)^=(Y);

#define RANGE(X, Y, Z) \
((X)<(Y)?(Y):((X)>(Z)?(Z):(X)))

#define SAFE_DELETE(XPointer) delete(XPointer); (XPointer)=nullptr;

#define CYCLE_ERASE(XVector, YIndex) \
	(XVector.at(YIndex) = XVector.at(XVector.size() - 1)); \
	(XVector.resize(XVector.size() - 1));

#define MAKE_ABSOLUTE(X) \
	((X)>=0)?(X):(-X);

#define NEGATE(X) \
	((X)>=0)?((X) = (-X)):(X);

#define OPPOSITE(X) \
	((X)>=0)?((X) = (-X)):((X) = (-X))
#pragma endregion
