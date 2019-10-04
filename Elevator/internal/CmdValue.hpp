#ifndef BOOST_MES_VAL_H
#define BOOST_MES_VAL_H

struct CmdValue{
	CmdValue(int v, int c)
	: val(v), cmd(c){}
	CmdValue(int c)
	: val(c), cmd(c){	}
	CmdValue()
	: val(0), cmd(0){}


	int val;
	int cmd;
};

inline
std::istream &operator >>(std::istream &is, CmdValue& msg) noexcept {
    is >> msg.val;
    is >> msg.cmd;

    return is;
}

#endif // BOOST_MES_VAL_H
