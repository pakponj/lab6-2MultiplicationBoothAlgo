#ifndef MULTIPLIER_H
#define MULTIPLIER_H

#include <systemc.h>
#include "alu.h"

SC_MODULE(multiplication) {
	sc_in<sc_uint<16> > multiplicand;	//input multiplicand
	sc_in<sc_uint<16> > multiplier;		//input multiplier
	sc_out<sc_uint<16> > A;				//output A
	sc_out<sc_uint<16> > Q;				//output Q
	sc_in<int> n;						//input count before termination
	
	sc_uint<16> q;						//stores multiplicand
	sc_uint<1> q_1;
	sc_uint<16> m;						//stores multiplier
	sc_uint<16> a;						//stores C
	int count;							//stores n

	sc_uint<1> carry;					//stores sign bit

	sc_signal<sc_uint<16> > ain, bin, sum;
	sc_signal<sc_uint<4> > opCode;
	sc_signal<bool> ci, co, oflag, zflag, ltflag;

	alu alu1;	//Instantiate alu module
	
	void printAQQ_1M() {
		for (int i = 15; i >= 0; i--) {
			cout << a[i];
		}
		cout << " ";
		for (int i = 15; i >= 0; i--) {
			cout << q[i];
		}
		cout << q_1 << " ";
		for (int i = 15; i >= 0; i--) {
			cout << m[i];
		}
		cout << endl;
	}

	void multiply()
	{
		//Read inputs
		q = multiplicand.read();
		m = multiplier.read();
		count = n.read();
		
		//initialize values
		q_1 = "0b0";
		a = "0b0000000000000000";
		carry = "0b0";

		cout << endl;
		cout << "New operation" << endl;
		//printCAQM();
		for (int i = count; i > 0; i--) {
			//printCAQM();
			if (q[0] ^ q_1) {
				if (q[0]) {
					// A = A-M
					ain.write(a);
					bin.write(m);
					opCode.write("0b0001");
					sc_start(100, SC_NS);
					cout << "Result :" << sum.read() << endl;
					sc_int<16> temp = a - m;
					cout << "Expected :" << temp << endl;
				}
				else {
					// A = A+M
					ain.write(a);
					bin.write(m);
					opCode.write("0b0000");
					sc_start(100, SC_NS);
					cout << "Result :" << sum.read() << endl;
					sc_int<16> temp = a + m;
					cout << "Expected :" << temp << endl;
				}
				/*ain.write(a);
				bin.write(m);
				ci.write(false);
				opCode.write("0b0000");
				sc_start(100 , SC_NS);
				carry = co.read();
				cout << "Result :" << sum.read() << endl;
				sc_int<16> temp = a + m;
				cout << "Expected :" << temp << endl;
				*/a = sum.read();
			}

			/*cout << "Before shifted" << endl;
			printCAQM();*/
			shiftRight();
			/*cout << "After shifted" << endl;
			printCAQM();*/
		}
		A.write(a);
		Q.write(q);
	}

	void shiftRight()
	{
		sc_uint<1> q0_temp = q[0];
		sc_uint<1> a0_temp = a[0];
		carry = 0;
		a = a >> 1;
		q = q >> 1;
		a[15] = a[14];
		q[15] = a0_temp;
		q_1 = q0_temp;
	}



	SC_CTOR(multiplication) : alu1("alu") {

		SC_METHOD(multiply);

		sensitive << multiplicand << multiplier;

		//Bind values
		alu1.ain(ain);
		alu1.bin(bin);
		alu1.ci(ci);
		alu1.opALU(opCode);
		alu1.sum(sum);
		alu1.co(co);
		alu1.oflag(oflag);
		alu1.zflag(zflag);
		alu1.ltflag(ltflag);
	}
};

#endif