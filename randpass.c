/*
** Copyright (c) 1999, 2000, 2001
** Adel I. Mirzazhanov. All rights reserved
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 
**     1.Redistributions of source code must retain the above copyright notice,
**       this list of conditions and the following disclaimer. 
**     2.Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution. 
**     3.The name of the author may not be used to endorse or promote products
**       derived from this software without specific prior written permission. 
** 		  
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR  ``AS IS'' AND ANY EXPRESS
** OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN  NO  EVENT  SHALL THE AUTHOR BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE
** GOODS OR SERVICES;  LOSS OF USE,  DATA,  OR  PROFITS;  OR BUSINESS
** INTERRUPTION)  HOWEVER  CAUSED  AND  ON  ANY  THEORY OF LIABILITY,
** WHETHER  IN  CONTRACT,   STRICT   LIABILITY,  OR  TORT  (INCLUDING
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
** randpass.c - Random password generation module of PWGEN program
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <unistd.h>
#include "randpass.h"
#include "owntypes.h"
struct sym
 {
  char   ch;
  USHORT type;
 };
static struct sym smbl[94] = 
{
 {'a', S_SL}, {'b', S_SL}, {'c', S_SL}, {'d', S_SL}, {'e', S_SL}, {'f', S_SL},
 {'g', S_SL}, {'h', S_SL}, {'i', S_SL}, {'j', S_SL}, {'k', S_SL}, {'l', S_SL},
 {'m', S_SL}, {'n', S_SL}, {'o', S_SL}, {'p', S_SL}, {'q', S_SL}, {'r', S_SL},
 {'s', S_SL}, {'t', S_SL}, {'u', S_SL}, {'v', S_SL}, {'w', S_SL}, {'x', S_SL},
 {'y', S_SL}, {'z', S_SL}, {'A', S_CL}, {'B', S_CL}, {'C', S_CL}, {'D', S_CL},
 {'E', S_CL}, {'F', S_CL}, {'G', S_CL}, {'H', S_CL}, {'I', S_CL}, {'I', S_CL},
 {'K', S_CL}, {'K', S_CL}, {'M', S_CL}, {'N', S_CL}, {'O', S_CL}, {'P', S_CL},
 {'Q', S_CL}, {'R', S_CL}, {'S', S_CL}, {'T', S_CL}, {'U', S_CL}, {'V', S_CL},
 {'W', S_CL}, {'X', S_CL}, {'Y', S_CL}, {'Z', S_CL}, {'1', S_NB}, {'2', S_NB},
 {'3', S_NB}, {'4', S_NB}, {'5', S_NB}, {'6', S_NB}, {'7', S_NB}, {'8', S_NB},
 {'9', S_NB}, {'0', S_NB}, {33 , S_SS}, {34 , S_SS|S_RS}, {35 , S_SS}, {36 , S_SS|S_RS},
 {37 , S_SS}, {38 , S_SS}, {39 , S_SS|S_RS}, {40 , S_SS}, {41 , S_SS}, {42 , S_SS},
 {43 , S_SS}, {44 , S_SS}, {45 , S_SS}, {46 , S_SS}, {47 , S_SS}, {58 , S_SS},
 {59 , S_SS}, {60 , S_SS}, {61 , S_SS}, {62 , S_SS}, {63 , S_SS|S_RS}, {64 , S_SS},
 {91 , S_SS}, {92 , S_SS|S_RS}, {93 , S_SS}, {94 , S_SS}, {95 , S_SS}, {96 , S_SS|S_RS},
 {123, S_SS}, {124, S_SS|S_RS}, {125, S_SS}, {126, S_SS}
};

/*
** gen_rand_pass - generates random password of specified type
*/
int
gen_rand_pass (char *password_string, int minl, int maxl, unsigned int pass_mode)
{
  int i = 0;
  int j = 0;
  int length = 0;
  char *str_pointer;
  int random_weight[94];
  int max_weight = 0;
  int max_weight_element_number = 0;

  if (minl > 256 || maxl > 256 || minl < 1 || maxl < 1 || minl > maxl)
      return (-1);
  for (i = 0; i <= 93; i++) random_weight[i] = 0; 
  length = minl + randint(maxl-minl+1);
  str_pointer = password_string;

  for (i = 0; i < length; i++)
    {
/* Asign random weight in weight array if mode is present*/
      for (j = 0; j <= 93 ; j++)
/*!!!*/  if ( ((pass_mode & smbl[j].type) > 0) &&
	     !((pass_mode & smbl[j].type) == 0x12))
	    random_weight[j] = 1 + randint(20000);
      j = 0;
/* Find an element with maximum weight */
      for (j = 0; j <= 93; j++)
	if (random_weight[j] > max_weight)
	  {
	    max_weight = random_weight[j];
	    max_weight_element_number = j;
	  }
/* Get password symbol */
      *str_pointer = smbl[max_weight_element_number].ch;
      str_pointer++;
      max_weight = 0;
      max_weight_element_number = 0;
      for (j = 0; j <= 93; j++) random_weight[j] = 0;
    }
  *str_pointer = 0;
  return (length);
}
