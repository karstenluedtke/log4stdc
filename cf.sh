CPPFLAGS="-I../fmcc/ThirdParty/i386-linux-64/include"	\
LDFLAGS="-L../fmcc/ThirdParty/i386-linux-64/lib"	\
./configure --prefix $HOME/fmcc/ThirdParty/host-native	\
	--enable-bfclog4cxx=auto
