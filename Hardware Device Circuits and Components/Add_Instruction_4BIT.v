// DSCH 3.5
// 09/11/2021 10:24:49
// C:\Users\BFTC\Desktop\D.S.O.S\Systems\PIC Assembly Programming\DataSheets & Tools\DSCH\Add_Instruction_4BIT.sch

module Add_Instruction_4BIT( A3,A2,A1,A0,B3,B2,B1,OP,
 B0,A7,A6,A4,A5,B7,B6,B4,
 B5,c77,c66,c55,c44,c33,c22,c11,
 c00,c00,c77,c66,c55,c44,c33,c22,
 c11,c00,[7],[6],[5],[4],[3],[2],
 [1],[0]);
 input A3,A2,A1,A0,B3,B2,B1,OP;
 input B0,A7,A6,A4,A5,B7,B6,B4;
 input B5;
 output c77,c66,c55,c44,c33,c22,c11,c00;
 output c00,c77,c66,c55,c44,c33,c22,c11;
 output c00,[7],[6],[5],[4],[3],[2],[1];
 output [0];
 wire w3,w4,w5,w7,w8,w9,w10,w11;
 wire w34,w35,w36,w37,w38,w39,w40,w41;
 wire w42,w43,w45,w46,w55,w56,w57,w58;
 wire w59,w60,w61,w62,w63,w64,w65,w66;
 wire w67,w68,w69,w70,w71,w72,w73;
 xor #(19) xor2_1(w4,A3,w3);
 xor #(18) xor2_2(c33,w5,w4);
 and #(18) and2_3(w7,A3,w3);
 and #(18) and2_4(w8,w4,w5);
 or #(19) or2_5(w9,w8,w7);
 or #(19) or2_6(w5,w10,w11);
 xor #(19) xor2_7(w34,OP,B1);
 and #(18) and2_8(w10,w35,w36);
 and #(18) and2_9(w11,A2,w37);
 xor #(18) xor2_10(c22,w36,w35);
 xor #(19) xor2_11(w35,A2,w37);
 or #(19) or2_12(w36,w38,w39);
 xor #(19) xor2_13(w3,OP,B3);
 and #(18) and2_14(w38,w40,w41);
 and #(18) and2_15(w39,A1,w34);
 xor #(18) xor2_16(c11,w41,w40);
 or #(19) or2_17(w41,w42,w43);
 xor #(19) xor2_18(w40,A1,w34);
 xor #(19) xor2_19(w45,OP,B0);
 and #(18) and2_20(w42,w46,OP);
 xor #(18) xor2_21(c00,OP,w46);
 and #(18) and2_22(w43,A0,w45);
 xor #(19) xor2_23(w37,OP,B2);
 xor #(19) xor2_24(w46,A0,w45);
 and #(2) and2_25(w56,[5],w55);
 xor #(2) xor2_26(c55,w57,w58);
 xor #(2) xor2_27(w58,[5],w55);
 xor #(2) xor2_28(c44,w9,w59);
 xor #(2) xor2_29(w59,[4],w60);
 and #(2) and2_30(w61,w58,w57);
 or #(2) or2_31(w57,w62,w63);
 xor #(2) xor2_32(w64,OP,c77);
 xor #(2) xor2_33(w60,OP,c44);
 and #(2) and2_34(w63,[4],w60);
 and #(2) and2_35(w62,w59,w9);
 or #(2) or2_36(w65,w61,w56);
 or #(2) or2_37(c00,w66,w67);
 and #(2) and2_38(w66,w68,w69);
 and #(2) and2_39(w67,[7],w64);
 xor #(2) xor2_40(c77,w69,w68);
 xor #(2) xor2_41(w68,[7],w64);
 xor #(2) xor2_42(w70,OP,c66);
 or #(2) or2_43(w69,w71,w72);
 and #(2) and2_44(w71,w73,w65);
 and #(2) and2_45(w72,[6],w70);
 xor #(2) xor2_46(w55,OP,c55);
 xor #(2) xor2_47(c66,w65,w73);
 xor #(2) xor2_48(w73,[6],w70);
endmodule

// Simulation parameters in Verilog Format
always
#200 A3=~A3;
#400 A2=~A2;
#800 A1=~A1;
#1600 A0=~A0;
#3200 B3=~B3;
#6400 B2=~B2;
#12800 B1=~B1;
#25600 OP=~OP;
#51200 B0=~B0;
#102400 A7=~A7;
#102400 A6=~A6;
#102400 A4=~A4;
#102400 A5=~A5;
#102400 B7=~B7;
#102400 B6=~B6;
#102400 B4=~B4;
#102400 B5=~B5;

// Simulation parameters
// A3 CLK 1 1
// A2 CLK 2 2
// A1 CLK 4 4
// A0 CLK 8 8
// B3 CLK 16 16
// B2 CLK 32 32
// B1 CLK 64 64
// OP CLK 128 128
// B0 CLK 256 256
// A7 CLK 512 512
// A6 CLK 1024 1024
// A4 CLK 2048 2048
// A5 CLK 4096 4096
// B7 CLK 8192 8192
// B6 CLK 16384 16384
// B4 CLK 32768 32768
// B5 CLK 32768 32768
