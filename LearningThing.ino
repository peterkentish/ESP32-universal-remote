/*
 * Learning thing is the class which listens using the expander board
 * for incoming IR codes and then decodes them. It is used to create
 * custom IR command buttons
 */

//learn code grabs IR codes and saves them to the fields learntCode remoteMake
bool learnCode(){
  bool changed = false;
  decode_results results;
  if (irrecv.decode(&results)) {  // Grab an IR code
    dumpInfo(&results);           // Output the results
    Serial.println("");           // Blank line between entries
    if (learntCode != results.value) {
      learntCode = results.value;
      changed = true;
      Serial.println(changed);
    }
    remoteMake = results.decode_type;
    irrecv.resume();              // Prepare for the next value
  }
  return changed;
}

//prints the info from IRReceive
void  dumpInfo (decode_results *results)
{
  // Check if the buffer overflowed
  if (results->overflow) {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWBUF");
    return;
  }

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  ircode(results);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

//prints the encoding type obtained by IR receive
void  encoding (decode_results *results)
{
  switch (results->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break ;
    case NEC:          Serial.print("NEC");           break ;
    case SONY:         Serial.print("SONY");          break ;
    case RC5:          Serial.print("RC5");           break ;
    case RC6:          Serial.print("RC6");           break ;
    case DISH:         Serial.print("DISH");          break ;
    case SHARP:        Serial.print("SHARP");         break ;
    case JVC:          Serial.print("JVC");           break ;
    case SANYO:        Serial.print("SANYO");         break ;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break ;
    case SAMSUNG:      Serial.print("SAMSUNG");       break ;
    case LG:           Serial.print("LG");            break ;
    case WHYNTER:      Serial.print("WHYNTER");       break ;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break ;
    case PANASONIC:    Serial.print("PANASONIC");     break ;
    case DENON:        Serial.print("Denon");         break ;
  }
}
//prints the IRCode obtained by IRReceive
void  ircode (decode_results *results)
{
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
    Serial.print(results->address, HEX);
    Serial.print(":");
  }

  // Print Code
  Serial.print(results->value, HEX);
}
