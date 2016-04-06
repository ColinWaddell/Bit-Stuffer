#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void print_b(uint8_t *in, uint32_t n);
uint32_t bit_stuffer( uint8_t *in, uint8_t *out, uint16_t n);

// Number of 1's which need a 0 appended
#define BIT_STUFF_ONES 5

// Print array 'in' with length 'N' as binary stream
void print_b(uint8_t *in, uint32_t N) {
  uint8_t b=7;
  uint16_t i=0;
  while(N--){
    putchar('0' + ((in[i]>> (b&0x07)) & 1));
    if(--b==0) i++;
  }
}

void bit_stuffer_bit_inc(uint8_t *b){

}

/* Takes input array 'in' of length 'N' and
 * outputs a bit stuffed version in 'out'.
 * Total number of bytes written during
 * stuffing returned by function.
 * Please ensure 'out' is long enough to
 * support the operation.
 *
 * A quick back of the envelope shows that
 * worst case senario for bit-stuff is all
 * 1's in in the input array. This would
 * place a 0 every 'BIT_STUFF_ONES' bit's
 * along. So we can see the output length
 * in bytes will always be at most:
 *
 *  out_length
 *    = in_length
 *    + in_length/BIT_STUFF_ONES
 *    + 1
 *
 * The +1 is for good luck.
 */
uint32_t bit_stuffer(
                     uint8_t *in,
                     uint8_t *out,
                     uint16_t N)
{
  uint16_t i      = 0;        // input byte counter
  uint16_t o      = 0xFFFF;   // output byte counter
  int8_t   b      = 0;        // input bit counter
  uint16_t out_b  = 7;        // bit position of output
  uint8_t  one_n  = 0;        // number of consequtive ones
  uint8_t  bit    = 0;        // current bit
  uint8_t first   = 1;
  uint32_t bit_count = 0;

  /* o is initialised with 0xFFFF to force
   * it to equal 0 on the first loop. This
   * ensures the correct number of bytes
   * are reported back.
   */

  /* Always clear the next output byte
   * so you don't have to 'write' zeros
   */
  out[0] = 0;

  // Input byte loop
  for(i=0; i<N; i++){
    // Input bit loop
    for(b=7; b>=0; b--){
      bit_count++;
      if(out_b==7){
        o = (o+1)&0xFFFF;
        out[o] = 0;
      }
      first = 0;
      bit = in[i]&(1<<b); // anything > 0 is a bit

      if (bit){
        // Bit is a 1
        out[o] |= (1<<out_b);
        one_n++;
        if(one_n==BIT_STUFF_ONES){
          // Update output counters to bit-stuff
          bit_count++;
          out_b = (out_b-1)&0x07;
          if(out_b==7){
            o = (o+1)&0xFFFF;
            out[o] = 0;
          }

          one_n = 0; // Reset counter
        }
      }
      else{
        // Bit is a 0
        // Reset counter
        one_n = 0;
      }
      // Update output counters
      out_b = (out_b-1)&0x07;
    }
  }

  return bit_count;
}

int main(void){
  // Sample data
  uint8_t in[] =
  { 0b00111110,
    0b00101010,
    0b10111111,
    0b10101000,
    0b11110000,
    0b00111111,
    0b10101111,
    0b10000000 };

  // See above for calculation for length of out.
  uint8_t out[sizeof(in)+(sizeof(in)/5)+1];
  // Length of output after bit-stuffing
  uint16_t out_n;

  // Print input data
  printf("input data:\n");
  print_b(in, sizeof(in)*8);

  // Stuff them bits
  out_n = bit_stuffer(in, out, sizeof(in));

  // Print output data
  printf("\noutput data:\n");
  print_b(out, out_n);
  printf("\nNumber of output bits: %d\n", out_n);

  return 0;
}

/* Console output:
 *
 * > input data:
 * > 0011111000101010101111111010100011110000001111111010111110000000
 * > output data:
 * > 001111100001010101011111011010100011110000001111101101011111000000000000
 *
 * Correct output:
 *   001111100001010101011111011010100011110000001111101101011111000000000000
 */
