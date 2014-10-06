#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void DisassembleChip8Op(uint8_t *codebuffer, int pc);

int main (int argc, char**argv)
{
  FILE *f= fopen(argv[1], "rb");
  if (f==NULL)
  {
      printf("error: Couldn't open %s\n", argv[1]);
      exit(1);
  }

  /* Get the file size */
  fseek(f, 0L, SEEK_END);
  int fsize = ftell(f);
  fseek(f, 0L, SEEK_SET);

  /* CHIP-8 convention puts programs in memory at 0x200
   * They will all have hardcoded addresses expecting that
   *
   * Read the file into memory at 0x200 and close it. */
  unsigned char *buffer=malloc(fsize+0x200);
  fread(buffer+0x200, fsize, 1, f);
  fclose(f);

  int pc = 0x200;
  while (pc < (fsize+0x200))
  {
	  DisassembleChip8Op(buffer, pc);
      pc += 2;
      printf ("\n");
  }
  return 0;
}

void DisassembleChip8Op(uint8_t *codebuffer, int pc)
{
  uint8_t *code = &codebuffer[pc];
  uint8_t firstnib = (code[0] >> 4);

  printf("%04x %02x %02x ", pc, code[0], code[1]);
  switch (firstnib)
  {
      case 0x00:
		  {
			  if (code[0] == 0 && code[1] == 0xE0)
			  {
				  /* Clears the screen */
				  printf("%-10s", "CLR");
			  }
			  else if (code[0] == 0 && code[1] == 0xEE)
			  {
				  /* Returns from a subroutine */
				  printf("%-10s", "RET");
			  }
			  else
			  {
				  /* Calls RCA 1802 program at address NNN */
				  uint8_t addresshi = code[0] & 0x0f;
				  printf("%-10s #$%01x%02x", "RCACLL", addresshi, code[1]);
			  }
		  }
		  break;
      case 0x01:
    	  {
    		  /* Jumps to address NNN */
    		  uint8_t addresshi = code[0] & 0x0f;
    		  printf("%-10s #$%01x%02x", "JMP", addresshi, code[1]);
    	  }
    	  break;
      case 0x02:
		  {
			  /* Calls subroutine at NNN */
			  uint8_t addresshi = code[0] & 0x0f;
			  printf("%-10s #$%01x%02x", "CALL", addresshi, code[1]);
		  }
		  break;
      case 0x03:
		  {
			  /* Skips the next instruction if VX equals NN */
			  uint8_t reg = code[0] & 0x0f;
			  printf("%-10s V%01X,#$%02x", "SKPEQ", reg, code[1]);
		  }
		  break;
      case 0x04:
		  {
			  /* Skips the next instruction if VX doesn't equal NN */
			  uint8_t reg = code[0] & 0x0f;
			  printf("%-10s V%01X,#$%02x", "SKPNE", reg, code[1]);
		  }
		  break;
      case 0x05:
		  {
			  if ((code[1] & 0x0f)!=0x0)
			  {
				  printf("Unrecognized OpCode");
			  }
			  else
			  {
				  /* Skips the next instruction if VX equals VY */
				  uint8_t reg_x = code[0] & 0x0f;
				  uint8_t reg_y = (code[1] >> 4);
				  printf("%-10s V%01X,V%01X", "SKPEQ", reg_x, reg_y);
			  }
		  }
		  break;
      case 0x06:
          {
        	  /* Sets VX to NN */
              uint8_t reg = code[0] & 0x0f;
              printf("%-10s V%01X,#$%02x", "MOV", reg, code[1]);
          }
          break;
      case 0x07:
    	  {
    		  /* Adds NN to VX */
    		  uint8_t reg = code[0] & 0x0f;
    		  printf("%-10s V%01X,#$%02x", "ADD", reg, code[1]);
    	  }
    	  break;
      case 0x08:
      	  {
      		uint8_t lastnib = code[1] & 0x0f;
      		switch (lastnib)
      		  {
      		      case 0x00:
					  {
						  /* Sets VX to the value of VY */
						  uint8_t reg_x = code[0] & 0x0f;
						  uint8_t reg_y = (code[1] >> 4);
						  printf("%-10s V%01X,V%01X", "MOV", reg_x, reg_y);
					  }
					  break;
      		      case 0x01:
					  {
						  /* Sets VX to the value of VY */
						  uint8_t reg_x = code[0] & 0x0f;
						  uint8_t reg_y = (code[1] >> 4);
						  printf("%-10s V%01X,V%01X", "OR", reg_x, reg_y);
					  }
					  break;
      		      case 0x02:
					  {
						  /* Sets VX to VX or VY. */
						  uint8_t reg_x = code[0] & 0x0f;
						  uint8_t reg_y = (code[1] >> 4);
						  printf("%-10s V%01X,V%01X", "AND", reg_x, reg_y);
					  }
					  break;
      		      case 0x03:
					  {
						  /* Sets VX to VX xor VY */
						  uint8_t reg_x = code[0] & 0x0f;
						  uint8_t reg_y = (code[1] >> 4);
						  printf("%-10s V%01X,V%01X", "XOR", reg_x, reg_y);
					  }
					  break;
      		      case 0x04:
					  {
						  /* Adds VY to VX.
						   * VF is set to 1 when there's a carry,
						   * and to 0 when there isn't */
						  uint8_t reg_x = code[0] & 0x0f;
						  uint8_t reg_y = (code[1] >> 4);
						  printf("%-10s V%01X,V%01X", "ADD", reg_x, reg_y);
					  }
					  break;
      		      case 0x05:
					  {
						  /* VY is subtracted from VX.
						   * VF is set to 0 when there's a borrow,
						   * and 1 when there isn't */
						  uint8_t reg_x = code[0] & 0x0f;
						  uint8_t reg_y = (code[1] >> 4);
						  printf("%-10s V%01X,V%01X", "SUB", reg_x, reg_y);
					  }
					  break;
      		      case 0x06:
					  {
						  /* Shifts VX right by one.
						   * VF is set to the value of the least
						   * significant bit of VX before the shift */
						  uint8_t reg_x = code[0] & 0x0f;
						  printf("%-10s V%01X", "SHR", reg_x);
					  }
					  break;
      		      case 0x07:
					  {
						  /* Sets VX to VY minus VX.
						   * VF is set to 0 when there's a borrow,
						   * and 1 when there isn't */
						  uint8_t reg_x = code[0] & 0x0f;
						  uint8_t reg_y = (code[1] >> 4);
						  printf("%-10s V%01X,V%01X", "RSUB", reg_x, reg_y);
					  }
					  break;
      		      case 0x0E:
					  {
						  /* Shifts VX left by one.
						   * VF is set to the value of the most
						   * significant bit of VX before the shift */
						  uint8_t reg_x = code[0] & 0x0f;
						  printf("%-10s V%01X", "SHL", reg_x);
					  }
					  break;
      		      default:
      		    	  printf("Unrecognized OpCode");
      		  }
      	  }
      	  break;
      case 0x09:
		  {
			  if ((code[1] & 0x0f)!=0x0)
			  {
				  printf("Unrecognized OpCode");
			  }
			  else
			  {
				  /* Skips the next instruction if VX doesn't equal VY */
				  uint8_t reg_x = code[0] & 0x0f;
				  uint8_t reg_y = (code[1] >> 4);
				  printf("%-10s V%01X,V%01X", "SKPNE", reg_x, reg_y);
			  }
		  }
		  break;
      case 0x0a:
          {
        	  /* Sets I to the address NNN */
              uint8_t addresshi = code[0] & 0x0f;
              printf("%-10s #$%01x%02x", "MOVI", addresshi, code[1]);
          }
          break;
      case 0x0b:
      	  {
			  /* Jumps to the address NNN plus V0 */
			  uint8_t addresshi = code[0] & 0x0f;
			  printf("%-10s #$%01x%02x", "JMPV0", addresshi, code[1]);
		  }
      	  break;
      case 0x0c:
      	  {
			  /* Sets VX to a random number and NN */
			  uint8_t reg = code[0] & 0x0f;
			  printf("%-10s V%01X,#$%02x", "RND", reg, code[1]);
		  }
      	  break;
      case 0x0d:
    	  {
    		  /* Sprites stored in memory at location
    		   * in index register (I), maximum 8bits
    		   * wide. Wraps around the screen. If when
    		   * drawn, clears a pixel, register VF is
    		   * set to 1 otherwise it is zero. All drawing
    		   * is XOR drawing (e.g. it toggles the screen pixels) */
    		  uint8_t reg_x = code[0] & 0x0f;
			  uint8_t reg_y = (code[1] >> 4);
			  uint8_t height = code[1] & 0x0f;
			  printf("%-10s V%01X,V%01X,%01X", "SPRITE", reg_x, reg_y, height);
    	  }
    	  break;
      case 0x0e:
      	  {
      		  if (code[1] == 0x9E)
      		  {
      			  /* Skips the next instruction if
      			   * the key stored in VX is pressed */
      			uint8_t reg = code[0] & 0x0f;
      			printf("%-10s V%01X", "SKPDN", reg);
      		  }
      		  else if (code[1] == 0xA1)
      		  {
      			/* Skips the next instruction if
      			 * the key stored in VX isn't pressed */
      			uint8_t reg = code[0] & 0x0f;
      			printf("%-10s V%01X", "SKPUP", reg);
      		  }
      		  else
      		  {
      			printf("Unrecognized OpCode");
      		  }
      	  }
      	  break;
      case 0x0f:
		{
			switch (code[1])
			{
			  case 0x07:
				  {
					  /* Sets VX to the value of the delay timer */
					  uint8_t reg = code[0] & 0x0f;
					  printf("%-10s V%01X", "GETDTMR", reg);
				  }
				  break;
			  case 0x0A:
				{
					/* A key press is awaited, and then stored in VX */
					uint8_t reg = code[0] & 0x0f;
					printf("%-10s V%01X", "GETKEY", reg);
				}
				break;
			  case 0x15:
				{
					/* Sets the delay timer to VX */
					uint8_t reg = code[0] & 0x0f;
					printf("%-10s V%01X", "SETDTMR", reg);
				}
				break;
			  case 0x18:
				{
					/* Sets the sound timer to VX */
					uint8_t reg = code[0] & 0x0f;
					printf("%-10s V%01X", "SETSTMR", reg);
				}
				break;
			  case 0x1E:
				{
					/* Adds VX to I */
					uint8_t reg = code[0] & 0x0f;
					printf("%-10s V%01X", "ADDI", reg);
				}
				break;
			  case 0x29:
				{
					/* Sets I to the location of the
					 * sprite for the character in VX.
					 * Characters 0-F (in hexadecimal)
					 * are represented by a 4x5 font */
					uint8_t reg = code[0] & 0x0f;
					printf("%-10s V%01X", "HXCHR", reg);
				}
				break;
			  case 0x33:
				{
					/* Stores the Binary-coded decimal
					 * representation of VX, with the most
					 * significant of three digits at the
					 * address in I, the middle digit at I
					 * plus 1, and the least significant digit
					 * at I plus 2. (In other words, take the
					 * decimal representation of VX, place the
					 * hundreds digit in memory at location in I,
					 * the tens digit at location I+1, and the
					 * ones digit at location I+2.) */
					uint8_t reg = code[0] & 0x0f;
					printf("%-10s V%01X", "BCD", reg);
				}
				break;
			  case 0x55:
				{
					/* Stores V0 to VX in memory
					 * starting at address I */
					uint8_t reg = code[0] & 0x0f;
					printf("%-10s VO,V%01X", "STR", reg);
				}
				break;
			  case 0x65:
				{
					/* Fills V0 to VX with values
					 * from memory starting at address I */
					uint8_t reg = code[0] & 0x0f;
					printf("%-10s VO,V%01X", "LOAD", reg);
				}
				break;
			  default:
			    printf("Unrecognized OpCode");
			}
		}
		break;
  }
}

