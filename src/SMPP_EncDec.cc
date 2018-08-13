/******************************************************************************
* Copyright (c) 2000-2018 Ericsson Telecom AB
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v2.0
* which accompanies this distribution, and is available at
* https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*
* Contributors:
*   Gabor Szalai - initial implementation and initial documentation
*   Harald Welte - Alert Notification support
******************************************************************************/
//
//  File:               SMPP_EncDec.cc
//  Rev:                R2A
//  Prodnr:             CNL 113 772
//  Reference:          [1] Short Message Peer to Peer Protocol Specification v3.4
//  Reference:          [2] Short Message Peer to Peer Protocol Specification v5.0
#include "SMPP_Types.hh"

namespace SMPP__Types {
  void decode_SMPP_Bind(TTCN_Buffer& input, SMPP__Bind& output);
  void decode_SMPP_Bind_resp(TTCN_Buffer& input, const unsigned int body_length,
    SMPP__Bind__resp& output);
  void decode_SMPP_SM(TTCN_Buffer& input, const unsigned int body_length, SMPP__SM& output);
  void decode_SMPP_Cancel(TTCN_Buffer& input, const unsigned int body_length, SMPP__Cancel& output);
  void decode_SMPP_Replace(TTCN_Buffer& input, const unsigned int body_length, SMPP__Replace& output);
  void decode_SMPP_SM_resp(TTCN_Buffer& input, const unsigned int body_length, SMPP__SM__resp& output);
  void decode_SMPP_Outbind(TTCN_Buffer& input, SMPP__Outbind& output);
  void decode_SMPP_MULTI(TTCN_Buffer& input, const unsigned int body_length, SMPP__MULTI& output);
  void decode_SMPP_MULTI_resp(TTCN_Buffer& input, const unsigned int body_length, SMPP__MULTI__resp& output);
  void decode_SMPP_AlertNotif(TTCN_Buffer& input, const unsigned int body_length, SMPP__AlertNotif& output);

  // puts '\0' octets at the end of C-Octetstrings
  void pad_C_Octetstrings(SMPP__PDU& data);
  void pad_C_Octetstrings(SMPP__optional__parameters& data);
  void crop_C_Octetstrings(SMPP__optional__parameters& data);
  void pad_C_Octetstrings(SMPP__Bind& data);
  void pad_C_Octetstrings(SMPP__Bind__resp& data);
  void pad_C_Octetstrings(SMPP__Outbind& data);
  void pad_C_Octetstrings(SMPP__Cancel& data);
  void pad_C_Octetstrings(SMPP__Replace& data);
  void pad_C_Octetstrings(SMPP__SM& data);
  void pad_C_Octetstrings(SMPP__SM__resp& data);
  void pad_C_Octetstrings(SMPP__MULTI& data);
  void pad_C_Octetstrings(SMPP__MULTI__resp& data);
  void pad_C_Octetstrings(SMPP__AlertNotif& data);
  
  void pad_C_Octetstring(CHARSTRING& data);
 
  unsigned int pick_os(TTCN_Buffer& buf, const unsigned int length, OCTETSTRING& output);
  void pick_bit1(TTCN_Buffer& buf, BITSTRING& output);

// picks a C-Octetstring from the buffer
// returns number of bytes extracted (including trailing null char)
  unsigned int pick_cos(TTCN_Buffer& buf, CHARSTRING& output);
  int pick_int(TTCN_Buffer& buf);

INTEGER f__msg__length(const OCTETSTRING& data)
{
    size_t msg_len = data.lengthof();
    if (msg_len < 4) return -1;
    const unsigned char *data_ptr = (const unsigned char *)data;
    size_t expected_len = (data_ptr[0] << 24) | (data_ptr[1] << 16) |
      (data_ptr[2] << 8) | data_ptr[3];
    return expected_len;
}

INTEGER f__decode__SMPP(const OCTETSTRING& data, SMPP__PDU&  pdu){

  TTCN_Buffer input=data;

  pdu.header().decode(SMPP__header_descr_, input, TTCN_EncDec::CT_RAW);

  // decode body
  
  if (pdu.header().command__id() == c__SMPP__command__id__bind__receiver) {
    decode_SMPP_Bind(input, pdu.body().bind__receiver());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__bind__receiver__resp) {
    decode_SMPP_Bind_resp(input, pdu.header().command__len() - 16, pdu.body().bind__receiver__resp());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__bind__transmitter) {
    decode_SMPP_Bind(input, pdu.body().bind__transmitter());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__bind__transmitter__resp) {
    decode_SMPP_Bind_resp(input, pdu.header().command__len() - 16, pdu.body().bind__transmitter__resp());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__submit__sm) {
    decode_SMPP_SM(input, pdu.header().command__len() - 16, pdu.body().submit__sm());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__submit__sm__resp) {
    decode_SMPP_SM_resp(input, pdu.header().command__len() - 16, pdu.body().submit__sm__resp());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__submit__multi) {
    decode_SMPP_MULTI(input, pdu.header().command__len() - 16, pdu.body().submit__multi());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__submit__multi__resp) {
    decode_SMPP_MULTI_resp(input, pdu.header().command__len() - 16, pdu.body().submit__multi__resp());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__bind__transceiver) {
    decode_SMPP_Bind(input, pdu.body().bind__transceiver());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__bind__transceiver__resp) {
    decode_SMPP_Bind_resp(input, pdu.header().command__len() - 16, pdu.body().bind__transceiver__resp());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__outbind) {
    decode_SMPP_Outbind(input, pdu.body().outbind());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__deliver__sm) {
    decode_SMPP_SM(input, pdu.header().command__len() - 16, pdu.body().deliver__sm());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__deliver__sm__resp) {
    decode_SMPP_SM_resp(input, pdu.header().command__len() - 16, pdu.body().deliver__sm__resp());
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__cancel__sm) {
    decode_SMPP_Cancel(input, pdu.header().command__len() - 16, pdu.body().cancel__sm());

  } else if (pdu.header().command__id() == c__SMPP__command__id__cancel__sm__resp) {
    pdu.body().cancel__sm__resp() = NULL_VALUE;

  } else if (pdu.header().command__id() == c__SMPP__command__id__replace__sm) {
    decode_SMPP_Replace(input, pdu.header().command__len() - 16, pdu.body().replace__sm());

  } else if (pdu.header().command__id() == c__SMPP__command__id__replace__sm__resp) {
    pdu.body().replace__sm__resp() = NULL_VALUE;

  } else if (pdu.header().command__id() == c__SMPP__command__id__unbind) {
    pdu.body().unbind() = NULL_VALUE;
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__unbind__resp) {
    pdu.body().unbind__resp() = NULL_VALUE;
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__enquire__link) {
    pdu.body().enquire__link() = NULL_VALUE;
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__enquire__link__resp) {
    pdu.body().enquire__link__resp() = NULL_VALUE;
    
  } else if (pdu.header().command__id() == c__SMPP__command__id__generic__nack){
    pdu.body().generic__nack() = NULL_VALUE;

  } else if (pdu.header().command__id() == c__SMPP__command__id__alert__notification) {
    decode_SMPP_AlertNotif(input, pdu.header().command__len() - 16, pdu.body().alert__notif());

  } else {
    TTCN_warning("Decoding of incoming message not implemented.");
    int body_length = pdu.header().command__len() - 16;
    if (body_length < 0)
      TTCN_warning("Unable to decode body of SMPP message (invalid command_len)");
    pdu.body().raw() = OCTETSTRING(body_length, input.get_read_data());
    input.set_pos(input.get_pos() + body_length);
  }

 
return 0;
}


void f__encode__SMPP(const SMPP__PDU& pdu, OCTETSTRING& data){
  SMPP__PDU to_padding = pdu;
  pad_C_Octetstrings(to_padding);
  TTCN_Buffer out_buf;
  to_padding.encode(SMPP__PDU_descr_, out_buf,
    TTCN_EncDec::CT_RAW);
  out_buf.get_string(data);
  
return;
}

void pad_C_Octetstrings(SMPP__PDU& data) {
  SMPP__operation__body& body = data.body();
  
  switch(body.get_selection()){
  case SMPP__operation__body::ALT_bind__transmitter: {
    pad_C_Octetstrings(body.bind__transmitter());
  }
  break;
  case SMPP__operation__body::ALT_bind__transmitter__resp: {
    pad_C_Octetstrings(body.bind__transmitter__resp());
  }
  break;
  case SMPP__operation__body::ALT_bind__receiver: {
    pad_C_Octetstrings(body.bind__receiver());
  }
  break;
  case SMPP__operation__body::ALT_bind__receiver__resp: {
    pad_C_Octetstrings(body.bind__receiver__resp());
  }
  break;
  case SMPP__operation__body::ALT_bind__transceiver: {
    pad_C_Octetstrings(body.bind__transceiver());
  }
  break;
  case SMPP__operation__body::ALT_bind__transceiver__resp: {
    pad_C_Octetstrings(body.bind__transceiver__resp());
  }
  break;
  case SMPP__operation__body::ALT_outbind: {
    pad_C_Octetstrings(body.outbind());
  }
  break;
  case SMPP__operation__body::ALT_unbind: {
    // void
  }
  break;
  case SMPP__operation__body::ALT_unbind__resp: {
    // void
  }
  break;
  case SMPP__operation__body::ALT_submit__sm: {
    pad_C_Octetstrings(body.submit__sm());
  }
  break;
  case SMPP__operation__body::ALT_submit__sm__resp: {
    pad_C_Octetstrings(body.submit__sm__resp());
  }
  break;
  case SMPP__operation__body::ALT_submit__multi: {
    pad_C_Octetstrings(body.submit__multi());
  }
  break;
  case SMPP__operation__body::ALT_submit__multi__resp: {
    pad_C_Octetstrings(body.submit__multi__resp());
  }
  break;
  case SMPP__operation__body::ALT_deliver__sm: {
    pad_C_Octetstrings(body.deliver__sm());
  }
  break;
  case SMPP__operation__body::ALT_deliver__sm__resp: {
    pad_C_Octetstrings(body.deliver__sm__resp());
  }
  break;
  case SMPP__operation__body::ALT_cancel__sm: {
    pad_C_Octetstrings(body.cancel__sm());
  }
  break;
  case SMPP__operation__body::ALT_cancel__sm__resp: {
    // void
  }
  break;
  case SMPP__operation__body::ALT_replace__sm: {
    pad_C_Octetstrings(body.replace__sm());
  }
  break;
  case SMPP__operation__body::ALT_replace__sm__resp: {
    // void
  }
  break;
  case SMPP__operation__body::ALT_enquire__link: {
    // void
  }
  break;
  case SMPP__operation__body::ALT_enquire__link__resp: {
    // void
  }
  break;
  case SMPP__operation__body::ALT_generic__nack: {
    // void 
  }
  break;
  case SMPP__operation__body::ALT_alert__notif: {
    pad_C_Octetstrings(body.alert__notif());
  }
  break;
  default:  
    TTCN_error("Unhandled union field in SMPP_operation_body.");
  }
}

void pad_C_Octetstrings(SMPP__SM& data) {
  pad_C_Octetstring(data.service__type());
  pad_C_Octetstring(data.source__addr());
  pad_C_Octetstring(data.destination__addr());
  pad_C_Octetstring(data.schedule__delivery__time());
  pad_C_Octetstring(data.validity__period());
  pad_C_Octetstrings(data.opt__pars());
}

void pad_C_Octetstrings(SMPP__SM__resp& data) {
  pad_C_Octetstring(data.message__id());
  if (data.opt__pars().ispresent())
    pad_C_Octetstrings(data.opt__pars());
}

void pad_C_Octetstrings(SMPP__MULTI& data) {
  pad_C_Octetstring(data.service__type());
  pad_C_Octetstring(data.source__addr());
  for(int a=0;a<data.dest__address().size_of();a++){
    if(data.dest__address()[a].dest__address().get_selection()==
                                        SMPP__SME__or__DL::ALT_sme__dest__addr){
      pad_C_Octetstring(data.dest__address()[a].dest__address().
                                         sme__dest__addr().destination__addr());
    } else {
      pad_C_Octetstring(data.dest__address()[a].dest__address().dl__name());
    }
  }
  pad_C_Octetstring(data.schedule__delivery__time());
  pad_C_Octetstring(data.validity__period());
  pad_C_Octetstrings(data.opt__pars());
}

void pad_C_Octetstrings(SMPP__MULTI__resp& data) {
  pad_C_Octetstring(data.message__id());
  for(int a=0;a<data.unsucecss__sme().size_of();a++){
      pad_C_Octetstring(data.unsucecss__sme()[a].destination__addr());
  }
  if (data.opt__pars().ispresent())
    pad_C_Octetstrings(data.opt__pars());
}


void pad_C_Octetstrings(SMPP__Bind& data){
  pad_C_Octetstring(data.system__id());
  pad_C_Octetstring(data.password());
  pad_C_Octetstring(data.system__type());
  pad_C_Octetstring(data.address__range());
}

void pad_C_Octetstrings(SMPP__Bind__resp& data){
  pad_C_Octetstring(data.system__id());
  pad_C_Octetstrings(data.opt__pars());
}

void pad_C_Octetstrings(SMPP__Outbind& data){
  pad_C_Octetstring(data.system__id());
  pad_C_Octetstring(data.password());
}

void pad_C_Octetstrings(SMPP__Cancel& data){
  pad_C_Octetstring(data.service__type());
  pad_C_Octetstring(data.message__id());
  pad_C_Octetstring(data.source__addr());
  pad_C_Octetstring(data.destination__addr());
}

void pad_C_Octetstrings(SMPP__Replace& data){
  pad_C_Octetstring(data.message__id());
  pad_C_Octetstring(data.source__addr());
  pad_C_Octetstring(data.schedule__delivery__time());
  pad_C_Octetstring(data.validity__period());
  pad_C_Octetstrings(data.opt__pars());
}

void pad_C_Octetstrings(SMPP__AlertNotif& data){
  pad_C_Octetstring(data.source__addr());
  pad_C_Octetstring(data.esme__addr());
  pad_C_Octetstrings(data.opt__pars());
}

void pad_C_Octetstrings(SMPP__optional__parameters& data){
  unsigned int num_pars = data.size_of();
  for (unsigned int par = 0; par < num_pars; par++) {
    // C-Octetstring optional parameters:
    // additional_status_info_text (0x001D)
    // receipted_message_id (0x001E)
    // source_network_id (0x060D)
    // dest_network_id (0x060E)
    if (data[par].tag() == SMPP__parameter__tag::additional__status__info__text 
      || data[par].tag() == SMPP__parameter__tag::receipted__message__id
      || data[par].tag() == SMPP__parameter__tag::source__network__id
      || data[par].tag() == SMPP__parameter__tag::dest__network__id) {
      if (data[par].opt__value().ispresent()) {
        SMPP__optional__value& val = (SMPP__optional__value&)data[par].opt__value();
        switch (val.get_selection()) {
        case SMPP__optional__value::ALT_char__val: {
          CHARSTRING& chr_val = val.char__val();
          chr_val = chr_val + CHARSTRING(1, "\0");
        }
        break;
        default:
          TTCN_warning("Invalid type of data in SMPP_optional_value %s.", 
            SMPP__parameter__tag::enum_to_str(data[par].tag()));
        }
      
      } else {
        TTCN_warning("Omitted data in SMPP_optional_value %s.", 
          SMPP__parameter__tag::enum_to_str(data[par].tag()));
      }
    }
  }
}

void crop_C_Octetstrings(SMPP__optional__parameters& data) {
  unsigned int num_pars = data.size_of();
  for (unsigned int par = 0; par < num_pars; par++) {
    // C-Octetstring optional parameters:
    // additional_status_info_text (0x001D)
    // receipted_message_id (0x001E)
    // source_network_id (0x060D)
    // dest_network_id (0x060E)    
    if (data[par].tag() == SMPP__parameter__tag::additional__status__info__text 
      || data[par].tag() == SMPP__parameter__tag::receipted__message__id
      || data[par].tag() == SMPP__parameter__tag::source__network__id
      || data[par].tag() == SMPP__parameter__tag::dest__network__id) {
      if (data[par].opt__value().ispresent()) {
        SMPP__optional__value& val = (SMPP__optional__value&)data[par].opt__value();
        switch (val.get_selection()) {
        case SMPP__optional__value::ALT_char__val: {
          CHARSTRING& chr_val = (CHARSTRING&)val.char__val();
          if (chr_val[chr_val.lengthof()-1] == CHARSTRING(1, "\0")){
            chr_val = substr(chr_val, 0, chr_val.lengthof()-1);
          } else {
            TTCN_warning("Last byte of parameter value %s is not zero.", 
              SMPP__parameter__tag::enum_to_str(data[par].tag()));
          }
        }
        break;
        default:
          TTCN_warning("Invalid type of data in SMPP_optional_value %s.", 
            SMPP__parameter__tag::enum_to_str(data[par].tag()));
        }
      } else {
        TTCN_warning("Omitted data in SMPP_optional_value %s.", 
          SMPP__parameter__tag::enum_to_str(data[par].tag()));
      }
    }
  }
}

void pad_C_Octetstring(CHARSTRING& data) {
  data = data + CHARSTRING(1, "\0");
}


void decode_SMPP_Bind(TTCN_Buffer& input, SMPP__Bind& output) {
  pick_cos(input, output.system__id());
  pick_cos(input, output.password());
  pick_cos(input, output.system__type());
  output.interface__version() = pick_int(input);
  output.addr__ton() = pick_int(input);
  output.addr__npi() = pick_int(input);
  pick_cos(input, output.address__range());
}

void decode_SMPP_SM_resp(TTCN_Buffer& input, const unsigned int body_length, SMPP__SM__resp& output) {
  unsigned int remaining_bytes = body_length;
  remaining_bytes -=  pick_cos(input, output.message__id());

  // optional parameters
  //my_log("REM: %d", remaining_bytes);
  if (remaining_bytes > 0) { // should be >=4 in this case anyway
    if (remaining_bytes < 4) {
      TTCN_warning("Invalid length of optional parameters.");
      output.opt__pars() = NULL_VALUE;
    } else {
      output.opt__pars()().decode(SMPP__optional__parameters_descr_, input, TTCN_EncDec::CT_RAW);
    }
  } else {
    output.opt__pars() = OMIT_VALUE;
  }
  if (output.opt__pars() != OMIT_VALUE)
    crop_C_Octetstrings(output.opt__pars());
}

void decode_SMPP_SM(TTCN_Buffer& input, const unsigned int body_length, SMPP__SM& output) {
  unsigned int remaining_bytes = body_length;

  remaining_bytes -= pick_cos(input, output.service__type());

  output.source__addr__ton() = pick_int(input);
  output.source__addr__npi() = pick_int(input);
  remaining_bytes -= 2;

  remaining_bytes -= pick_cos(input, output.source__addr());

  output.dest__addr__ton() = pick_int(input);
  output.dest__addr__npi() = pick_int(input);
  remaining_bytes -= 2;
  
  remaining_bytes -= pick_cos(input, output.destination__addr());

  pick_bit1(input, output.esm__class());
  output.protocol__id() = pick_int(input);
  output.priority__flag() = pick_int(input);
  remaining_bytes -= 3;
  
  remaining_bytes -= pick_cos(input, output.schedule__delivery__time());
  remaining_bytes -= pick_cos(input, output.validity__period());
  
  pick_bit1(input, output.registered__delivery());
  output.replace__if__present() = pick_int(input);
  pick_bit1(input, output.data__coding());
  output.sm__default__msg__id() = pick_int(input);
  output.sm__length() = pick_int(input);
  remaining_bytes -= 5;
  
  remaining_bytes -= pick_os(input, output.sm__length(), output.short__message());
  
  // optional parameters
  
  if (remaining_bytes > 0) { // should be >=4 in this case anyway
    if (remaining_bytes < 4) {
      TTCN_warning("Invalid length of optional parameters.");
      output.opt__pars() = NULL_VALUE;
    } else {
      output.opt__pars().decode(SMPP__optional__parameters_descr_, input, TTCN_EncDec::CT_RAW);
    }
  } else {
    output.opt__pars() = NULL_VALUE;
  }
  crop_C_Octetstrings(output.opt__pars());
  
}

void decode_SMPP_MULTI_resp(TTCN_Buffer& input, const unsigned int body_length, SMPP__MULTI__resp& output) {
  unsigned int remaining_bytes = body_length;
  remaining_bytes -=  pick_cos(input, output.message__id());
  output.no__unsuccess() = pick_int(input);
  remaining_bytes--;

  output.unsucecss__sme() = NULL_VALUE;
  for(int a=0;a<output.no__unsuccess();a++){
      output.unsucecss__sme()[a].dest__addr__ton() = pick_int(input);
      output.unsucecss__sme()[a].dest__addr__npi() = pick_int(input);
      remaining_bytes -= 2;
      remaining_bytes -= pick_cos(input, output.unsucecss__sme()[a].destination__addr());
      unsigned int tmp=pick_int(input);
      tmp=(tmp<<8) | pick_int(input);
      tmp=(tmp<<8) | pick_int(input);
      tmp=(tmp<<8) | pick_int(input);
      remaining_bytes -= 4;
      output.unsucecss__sme()[a].error__status__code()=tmp;
  }
  // optional parameters
  //my_log("REM: %d", remaining_bytes);
  if (remaining_bytes > 0) { // should be >=4 in this case anyway
    if (remaining_bytes < 4) {
      TTCN_warning("Invalid length of optional parameters.");
      output.opt__pars() = NULL_VALUE;
    } else {
      output.opt__pars()().decode(SMPP__optional__parameters_descr_, input, TTCN_EncDec::CT_RAW);
    }
  } else {
    output.opt__pars() = OMIT_VALUE;
  }
  if (output.opt__pars() != OMIT_VALUE)
    crop_C_Octetstrings(output.opt__pars());
}

void decode_SMPP_MULTI(TTCN_Buffer& input, const unsigned int body_length, SMPP__MULTI& output) {
  unsigned int remaining_bytes = body_length;

  remaining_bytes -= pick_cos(input, output.service__type());

  output.source__addr__ton() = pick_int(input);
  output.source__addr__npi() = pick_int(input);
  remaining_bytes -= 2;

  remaining_bytes -= pick_cos(input, output.source__addr());

  output.number__of__dests() = pick_int(input);
  remaining_bytes--;

  output.dest__address() = NULL_VALUE;
  for(int a=0;a<output.number__of__dests();a++){
    output.dest__address()[a].dest__flag() = pick_int(input);
    remaining_bytes--;
    if(output.dest__address()[a].dest__flag()==SMPP__DEST__flag::SME__address){
      output.dest__address()[a].dest__address().sme__dest__addr().dest__addr__ton() = pick_int(input);
      output.dest__address()[a].dest__address().sme__dest__addr().dest__addr__npi() = pick_int(input);
      remaining_bytes -= 2;
      remaining_bytes -= pick_cos(input, output.dest__address()[a].dest__address().sme__dest__addr().destination__addr());
    } else {
      remaining_bytes -= pick_cos(input, output.dest__address()[a].dest__address().dl__name());
    }
  }
  pick_bit1(input, output.esm__class());
  output.protocol__id() = pick_int(input);
  output.priority__flag() = pick_int(input);
  remaining_bytes -= 3;
  
  remaining_bytes -= pick_cos(input, output.schedule__delivery__time());
  remaining_bytes -= pick_cos(input, output.validity__period());
  
  pick_bit1(input, output.registered__delivery());
  output.replace__if__present() = pick_int(input);
  pick_bit1(input, output.data__coding());
  output.sm__default__msg__id() = pick_int(input);
  output.sm__length() = pick_int(input);
  remaining_bytes -= 5;
  
  remaining_bytes -= pick_os(input, output.sm__length(), output.short__message());
  
  // optional parameters
  
  if (remaining_bytes > 0) { // should be >=4 in this case anyway
    if (remaining_bytes < 4) {
      TTCN_warning("Invalid length of optional parameters.");
      output.opt__pars() = NULL_VALUE;
    } else {
      output.opt__pars().decode(SMPP__optional__parameters_descr_, input, TTCN_EncDec::CT_RAW);
    }
  } else {
    output.opt__pars() = NULL_VALUE;
  }
  crop_C_Octetstrings(output.opt__pars());
  
}



void decode_SMPP_Cancel(TTCN_Buffer& input, const unsigned int body_length, SMPP__Cancel& output) {
  unsigned int remaining_bytes = body_length;

  remaining_bytes -= pick_cos(input, output.service__type());
  remaining_bytes -= pick_cos(input, output.message__id());

  output.source__addr__ton() = pick_int(input);
  output.source__addr__npi() = pick_int(input);
  remaining_bytes -= 2;

  remaining_bytes -= pick_cos(input, output.source__addr());

  output.dest__addr__ton() = pick_int(input);
  output.dest__addr__npi() = pick_int(input);
  remaining_bytes -= 2;
  
  remaining_bytes -= pick_cos(input, output.destination__addr());
}

void decode_SMPP_Replace(TTCN_Buffer& input, const unsigned int body_length, SMPP__Replace& output) {
  unsigned int remaining_bytes = body_length;

  remaining_bytes -= pick_cos(input, output.message__id());

  output.source__addr__ton() = pick_int(input);
  output.source__addr__npi() = pick_int(input);
  remaining_bytes -= 2;
  
  remaining_bytes -= pick_cos(input, output.source__addr());
  remaining_bytes -= pick_cos(input, output.schedule__delivery__time());
  remaining_bytes -= pick_cos(input, output.validity__period());

  output.registered__delivery() = pick_int(input);
  output.sm__default__msg__id() = pick_int(input);
  output.sm__length() = pick_int(input);
  remaining_bytes -= 3;

  remaining_bytes -= pick_os(input, output.sm__length(), output.short__message());
  
  // optional parameters
  
  if (remaining_bytes > 0) { // should be >=4 in this case anyway
    if (remaining_bytes < 4) {
      TTCN_warning("Invalid length of optional parameters.");
      output.opt__pars() = NULL_VALUE;
    } else {
      output.opt__pars().decode(SMPP__optional__parameters_descr_, input, TTCN_EncDec::CT_RAW);
    }
  } else {
    output.opt__pars() = NULL_VALUE;
  }
  crop_C_Octetstrings(output.opt__pars());
}

void decode_SMPP_AlertNotif(TTCN_Buffer& input, const unsigned int body_length,
			    SMPP__AlertNotif& output) {
  unsigned int remaining_bytes = body_length;
  output.source__addr__ton() = pick_int(input);
  output.source__addr__npi() = pick_int(input);
  remaining_bytes -= 2;
  remaining_bytes -= pick_cos(input, output.source__addr());
  output.esme__addr__ton() = pick_int(input);
  output.esme__addr__npi() = pick_int(input);
  remaining_bytes -= 2;
  remaining_bytes -= pick_cos(input, output.esme__addr());

  if (remaining_bytes > 0) { // should be >=4 in this case anyway
    if (remaining_bytes < 4) {
      TTCN_warning("Invalid length of optional parameters.");
      output.opt__pars() = NULL_VALUE;
    } else {
      output.opt__pars().decode(SMPP__optional__parameters_descr_, input, TTCN_EncDec::CT_RAW);
    }
  } else {
    output.opt__pars() = NULL_VALUE;
  }
  crop_C_Octetstrings(output.opt__pars());
}

void decode_SMPP_Bind_resp(TTCN_Buffer& input, const unsigned int body_length, 
  SMPP__Bind__resp& output) {
  unsigned int remaining_bytes = body_length;
  remaining_bytes -= pick_cos(input, output.system__id());
  if (remaining_bytes > 0) { // should be >=4 in this case anyway
    if (remaining_bytes < 4) {
      TTCN_warning("Invalid length of optional parameters.");
      output.opt__pars() = NULL_VALUE;
    } else {
      output.opt__pars().decode(SMPP__optional__parameters_descr_, input, TTCN_EncDec::CT_RAW);
    }
  } else {
    output.opt__pars() = NULL_VALUE;
  }
  crop_C_Octetstrings(output.opt__pars());

}

void decode_SMPP_Outbind(TTCN_Buffer& input, SMPP__Outbind& output) {
  pick_cos(input, output.system__id());  
  pick_cos(input, output.password());
}

// picks an octetstring from the buffer
unsigned int pick_os(TTCN_Buffer& buf, const unsigned int length, OCTETSTRING& output) {
  const unsigned int read_len = buf.get_read_len();
  unsigned int extracted_bytes;
  if (read_len < length) {
    TTCN_warning("Can not extract octetstring of length %d from buffer", length);
    output = OCTETSTRING(read_len, buf.get_read_data());
    buf.set_pos(buf.get_pos() + read_len);
    extracted_bytes = read_len;
  } else {
    output = OCTETSTRING(length, buf.get_read_data());
    buf.set_pos(buf.get_pos() + length);
    extracted_bytes = length;
  }
  return extracted_bytes;
}

// picks a C-Octetstring from the buffer
// returns number of bytes extracted (including trailing null char)
unsigned int pick_cos(TTCN_Buffer& buf, CHARSTRING& output) {
  const unsigned int read_len = buf.get_read_len();
  unsigned int cur_pos = 0;
  const unsigned char* data = buf.get_read_data();
  
  while (data[cur_pos] != '\0') {
    cur_pos++;
    if (cur_pos >= read_len) {
      TTCN_warning("Failed to extract C-Octetstring from the buffer.");
      cur_pos--;
      break;
    }
  }
  
  // now cur_pos points to the trailing 0 byte
  cur_pos++;
  output = CHARSTRING(cur_pos - 1, (const char*)data);
  
  buf.set_pos(buf.get_pos() + cur_pos);
  return cur_pos;
}

// picks bitstring of length 8 (msb first)
void pick_bit1(TTCN_Buffer& buf, BITSTRING& output) {
  if (buf.get_read_len() < 1) {
    TTCN_warning("Failed to extract bitstring (8 bits) from buffer.");
    output = BITSTRING(8, (const unsigned char*)"\0");
  } else {
    output.decode(SMPP__BIT1_descr_, buf, TTCN_EncDec::CT_RAW);
  }
}

int pick_int(TTCN_Buffer& buf) {
  if (buf.get_read_len() < 1) {
    TTCN_warning("Failed to extract integer from buffer.");
    return -1;
  }
  int ret_val = *(buf.get_read_data());
  buf.set_pos(buf.get_pos() + 1);
  return ret_val;
}

}
