RECUPERACAO ORG. COMP. ABRIL/2018
        Guilherme dos Reis Meneguello - 9313708
/*
O trabalho abaixo utiliza como base o trabalho entregue pelo grupo 16.

/* code-c - Student's code for mmcpu

   Copyright 2017  Monaco F. J.   <monaco@icmc.usp.br>

   This file is part of Muticlycle Mips CPU (MMCPU)

   MMCPU is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "cpu.h"
#include "mask.h"

/* Define todos os estados que serao utilizados */
#define state_0   0x9408   // 1001 0100 0000 1000
#define state_1   0x0018   // 0000 0000 0001 1000
#define state_2   0x0014   // 0000 0000 0001 0100
#define state_3   0x1800   // 0001 1000 0000 0000
#define state_4   0x4002   // 0100 0000 0000 0010
#define state_5   0x0802   // 0000 1000 0000 0010
#define state_6   0x0044   // 0000 0000 0100 0100
#define state_7   0x0003   // 0000 0000 0000 0011
#define state_8   0x02a4   // 0000 0010 1010 0100
#define state_9   0x0488   // 0000 0100 1000 1000
#define state_10  0x0002   // 0000 0000 0000 0010
#define state_11  0x0082   // 0000 0000 1000 0010
#define state_12  0x02e4   // 0000 0010 1110 0100

/*Variaveis globais, 'zero' nao eh passado como parametro em algumas funcoes mas precisa ser utilizado;
    'overflow' nao foi utilizado*/
char zero;
char overflow;

/* Students, you are required to implemented the functions bellow.
   Please, refere to cpu.h for further information. */

/* Executa a acao necessaria de acordo com o alu_op recebido */
int alu(int a, int b, char alu_op, int *result_alu, char *zero, char *overflow)
{

    switch(alu_op)
    {
        // SOMA
        case ativa_soma:
            *result_alu = a+b;
            break;

        // SUBTRACAO
        case ativa_subtracao:
            *result_alu = a-b;
            break;

        // OR
        case ativa_or:
            *result_alu = a|b;
            break;

        // AND
        case ativa_and:
            *result_alu = a&b;
            break;

        // SLT
        case ativa_slt:
            if(a < b)
            {
                *result_alu = 0x01;
            }
            else
            {
                *result_alu = 0x00;
            }
            break;

        // alu_op invalido
        default:
            return -1;
            break;
    }

    // Ativa o bit_zero caso o resultado seja 0, se nao desativa
    if(*result_alu == 0)
    {
        *zero = ativa_bit_zero;
    }
    else
    {
        *zero = desativa_bit_zero;
    }

    return 0;
}

/* Unidade de controle da CPU */
void control_unit(int IR, short int *sc)
{
    if(IR == -1)
    {
        *sc = state_0;
    }
    else
    {
        char opCode = ((IR & separa_cop) >> 26);
        char rm = ((separa_rm & IR) >> 16);

        if(*sc == ((short int)state_0))
        {
            *sc = state_1;
        }
        else
        {
            switch(opCode)
            {
                case (0x00):  /* INSTRUÇÕES TIPO-R
                    switch(*sc)
                    {
                        case ((short int)state_1):
                            *sc = state_6;
                            break;
                        case ((short int)state_6):
                            *sc = state_7;
                            break;
                        case ((short int)state_7):
                            *sc = state_0;
                            break;
                    }
                    break;

                case (0x23):  /* LOAD WORD
                    switch(*sc)
                    {
                        case ((short int)state_1):
                            *sc = state_2;
                            break;
                        case ((short int)state_2):
                            *sc = state_3;
                            break;
                        case ((short int)state_3):
                            *sc = state_4;
                            break;
                        case ((short int)state_4):
                            *sc = state_0;
                            break;
                    }
                    break;

                case (0x2b):  // SW
                    switch(*sc)
                    {
                        case ((short int)state_1):
                            *sc = state_2;
                            break;
                        case ((short int)state_2):
                            *sc = state_5;
                            break;
                        case ((short int)state_5):
                            *sc = state_0;
                            break;
                    }
                    break;

                case (0x04):   /* BRANCH ON EQUAL
                    switch(*sc)
                    {
                        case ((short int)state_1):
                            *sc = state_8;
                            break;
                        case ((short int)state_8):
                            *sc = state_0;
                            break;
                    }
                    break;

                case (0x01):   /* BRANCH ON LESS THAN ZERO
                    if(rm == 0x00)
                    {
                        switch(*sc)
                            {
                                case ((short int)state_1):
                                    *sc = state_12;
                                    break;
                                case ((short int)state_12):
                                    *sc = state_0;
                                    break;
                            }
                    }else if(rm == 0x02)
                    {
                        switch(*sc)
                            {
                                case ((short int)state_1):
                                    *sc = state_12;
                                    break;
                                case ((short int)state_12):
                                    *sc = state_11;
                                    break;
                                case ((short int)state_11):
                                    *sc = state_0;
                                    break;
                            }
                    }
                    break;

                case (0x02):  /* JUMP
                    switch(*sc)
                    {
                        case ((short int)state_1):
                            *sc = state_9;
                            break;
                        case ((short int)state_9):
                            *sc = state_0;
                            break;
                    }
                    break;

                case (0x03):   /* JUMP AND LINK
                    switch(*sc)
                    {
                        case ((short int)state_1):
                            *sc = state_9;
                            break;
                        case ((short int)state_9):
                            *sc = state_11;
                            break;
                        case ((short int)state_11):
                            *sc = state_0;
                            break;
                    }
                    break;

                case (0x08):  /* ADD-I
                    switch(*sc)
                    {
                        case ((short int)state_1):
                            *sc = state_2;
                            break;
                        case ((short int)state_2):
                            *sc = state_10;
                            break;
                        case ((short int)state_10):
                            *sc = state_0;
                            break;
                    }
                    break;

                default:
                    return;
                    break;
            }
        }
    }
}

/* Busca a proxima instrucao a ser executada */
void instruction_fetch(short int sc, int PC, int ALUOUT, int IR, int* PCnew, int* IRnew, int* MDRnew)
{

    if(sc == ((short int)state_0))
    {
        *IRnew = memory[PC];
        char alu_op;

        // Incrementa PC
        alu_control(IR, sc, &alu_op);
        alu(PC, 1, alu_op, &ALUOUT, &zero, &overflow);

        *PCnew = ALUOUT;
        *MDRnew = memory[PC];

        if(*IRnew == 0)
        {
			loop = 0;
 			return;
		}
    }

    return;

}

/* Decodifica a instrucao quando esta no estado 1 */
void decode_register(short int sc, int IR, int PC, int A, int B, int *Anew, int *Bnew, int *ALUOUTnew)
{

    if(sc == ((short int)state_1))
    {
        char alu_op;

        *Anew = reg[(separa_rs & IR) >> 21];
        *Bnew = reg[(separa_rm & IR) >> 16];

        alu_control(IR, sc, &alu_op);
        alu(PC, (IR & separa_imediato), alu_op, ALUOUTnew, &zero, &overflow);
    }

    return;
}

/* Calculando branch caso necessario */
void exec_calc_end_branch(short int sc, int A, int B, int IR, int PC, int ALUOUT, int *ALUOUTnew, int *PCnew)
{
    char alu_op;

    alu_control(IR, sc, &alu_op);

    switch(sc)
    {
        // LW ou SW ou ADD-I
        case ((short int)state_2):
            alu(A, IR & separa_imediato, alu_op, ALUOUTnew, &zero, &overflow);
            break;

        // Tipo-R
        case ((short int)state_6):
            alu(A, B, alu_op, ALUOUTnew, &zero, &overflow);
            break;

        // BEQ
        case ((short int)state_8):
            alu(A, B, alu_op, ALUOUTnew, &zero, &overflow);
            if(zero == 1)
            {
                *PCnew = ALUOUT;
            }else
                return;
            break;

        // BLTZ
        case ((short int)state_12):
            alu(A, 0, alu_op, ALUOUTnew, &zero, &overflow);
            if(zero == 1)
            {
                return;
            }else{
                *PCnew = ALUOUT;
                *ALUOUTnew = PC;
            }
            break;

        // JUMP
        case ((short int)state_9):
            *PCnew = ((PC & separa_4bits_PC) | (IR & separa_endereco_jump));
            alu(PC, 0, alu_op, ALUOUTnew, &zero, &overflow);
            break;

        // NAO REQUISITADO
        default:
            break;
    }

    return;
}

/* Operacoes na memoria */
void write_r_access_memory(short int sc, int B, int IR, int ALUOUT, int PC, int *MDRnew, int *IRnew)
{

    switch(sc)
    {
        // LW
        case ((short int)state_3):
            *MDRnew = memory[ALUOUT];
            break;

        // SW
        case ((short int)state_5):
            memory[ALUOUT] = reg[(IR & separa_rm) >> 16];
            break;

        // TIPO - R
        case ((short int)state_7):
            reg[(IR & separa_rd) >> 11] = ALUOUT;
            break;

        // ADD- I
        case ((short int)state_10):
            reg[(IR & separa_rm) >> 16] = ALUOUT;
            break;

        // LINK
        case ((short int)state_11):
            reg[31] = ALUOUT;
            break;

        // NAO REQUISITADO
        default:
            break;
    }

    return;
}

/* Escreve em algum registrador */
void write_ref_mem(short int sc, int IR, int MDR, int ALUOUT)
{
    // APENAS LW
    if(sc == ((short int)state_4))
    {
        reg[(IR & separa_rm) >> 16] = MDR;
    }
    return;
}

/* Calcula o valor de alu_op */
void alu_control(int IR, int sc, char *alu_op)
{

    switch(((sc & separa_ALUOp0) | (sc & separa_ALUOp1)) >> 5)
    {
        // TIPO-R
        case 0x2:
            switch(IR & 0x0f)
            {
                // ADD
                case 0b0000:
                    *alu_op = 0b0010;
                    break;
                // SUB
                case 0b0010:
                    *alu_op = 0b0110;
                    break;
                // AND
                case 0b0100:
                    *alu_op = 0b0000;
                    break;
                // OR
                case 0b0101:
                    *alu_op = 0b0001;
                    break;
                // SLT
                case 0b1010:
                    *alu_op = 0b0111;
                    break;
                // INVALIDO
                default:
                    break;
            }
            break;

        //SW ou LW
        case 0x0:
            *alu_op = 0b0010;
            break;

        // BRANCH
        case 0x1:
            *alu_op = 0b0110;
            break;

       // BLTZ
        case 0x3:
            *alu_op = 0b0111;
            break;


        // INVALIDO
        default:
            return;
    }

}
