const int kPinPwm1 = 9;
const int kPwmPeriodMs = 10;

const int kYuugenLen = 1262;
extern const uint32_t kYuugenData[1262] PROGMEM;

const int kPhaseLen = 8;
const int kPhaseMin = 0;
const int kPhaseDon[kPhaseLen] = {4, 4, 3, 3, 2, 2, 2, 2};
const int kPhaseKatsu[kPhaseLen] = {10, 10, 10, 0, 0, 0, 0, 0};

unsigned long ts_base = 0;
int note_off = 0;
bool is_katsu = 0;
int phase_off = kPhaseLen;
unsigned long now;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(kPinPwm1, OUTPUT);

  ts_base = millis();
  now = ts_base;
}

struct Note {
  unsigned long ts;
  bool is_katsu;
};

struct Note getNote(int off) {
  uint32_t raw = pgm_read_dword(&(kYuugenData[off]));
  return Note{
    .ts = raw & 0x00ffffff,
    .is_katsu = (raw >> 24) > 2,
  };
}

void sleepUntil(unsigned long until) {
  while (millis() - until >= 0x80000000) {}
}

void loop() {
  unsigned long ts = now - ts_base;

  if (note_off < kYuugenLen) {
    auto note = getNote(note_off);
    if (ts + kPwmPeriodMs > note.ts) {
      digitalWrite(LED_BUILTIN, HIGH);
      is_katsu = note.is_katsu;
      phase_off = 0;
      ++note_off;
    }
  }

  int phase_val = phase_off == kPhaseLen ? kPhaseMin :
    is_katsu ? kPhaseKatsu[phase_off] : kPhaseDon[phase_off];

  digitalWrite(kPinPwm1, HIGH);
  sleepUntil(now + phase_val);

  digitalWrite(kPinPwm1, LOW);
  sleepUntil(now + kPhaseLen);

  digitalWrite(LED_BUILTIN, LOW);

  now += kPhaseLen;
  if (phase_off < kPhaseLen) ++phase_off;

  if (note_off == kYuugenLen && phase_off == kPhaseLen) while (true) { millis(); }
}

const uint32_t kYuugenData[1262] PROGMEM = {
0x01000000,
0x0200007d,
0x010000fa,
0x02000177,
0x010002ee,
0x0200036b,
0x010003e8,
0x02000465,
0x010005dc,
0x02000659,
0x010006d6,
0x02000753,
0x010008ca,
0x02000947,
0x010009c4,
0x03000a41,
0x00000abe,
0x03000b3b,
0x01000bb8,
0x02000c35,
0x01000cb2,
0x02000d2f,
0x01000ea6,
0x02000f23,
0x01000fa0,
0x0200101d,
0x01001194,
0x02001211,
0x0100128e,
0x0200130b,
0x00001482,
0x030014ff,
0x0100157c,
0x030015f9,
0x00001676,
0x020016f3,
0x01001770,
0x020017ed,
0x0100186a,
0x020018e7,
0x01001a5e,
0x02001adb,
0x01001b58,
0x02001bd5,
0x01001d4c,
0x02001dc9,
0x01001e46,
0x02001ec3,
0x0100203a,
0x020020b7,
0x01002134,
0x030021b1,
0x0000222e,
0x030022ab,
0x01002328,
0x020023a5,
0x01002422,
0x0200249f,
0x01002616,
0x02002693,
0x01002710,
0x0200278d,
0x01002904,
0x02002981,
0x010029fe,
0x02002a7b,
0x00002bf2,
0x03002c6f,
0x01002cec,
0x03002d69,
0x01002de6,
0x03002e63,
0x00002ee0,
0x02002f5d,
0x01002fda,
0x02003057,
0x000030d4,
0x02003151,
0x000031ce,
0x0200324b,
0x000032c8,
0x02003345,
0x000033c2,
0x0200343f,
0x000034bc,
0x02003539,
0x000035b6,
0x03003633,
0x010036b0,
0x0300372d,
0x000037aa,
0x02003827,
0x000038a4,
0x02003921,
0x0000399e,
0x02003a1b,
0x00004074,
0x020040d8,
0x0000413c,
0x020041a0,
0x00004204,
0x02004268,
0x0100429a,
0x03004330,
0x00004394,
0x030043f8,
0x0000445c,
0x020044c0,
0x00004524,
0x03004588,
0x010045ec,
0x0200461e,
0x00004650,
0x030046b4,
0x01004718,
0x0300477c,
0x010047e0,
0x03004844,
0x010048a8,
0x020048da,
0x00004970,
0x030049d4,
0x00004a38,
0x02004a9c,
0x00004b00,
0x03004b64,
0x01004bc8,
0x02004c2c,
0x01004c5e,
0x03004c90,
0x00004cf4,
0x02004d58,
0x00004dbc,
0x02004e20,
0x00004e84,
0x02004ee8,
0x01004f4c,
0x03004fb0,
0x00005014,
0x02005078,
0x010050dc,
0x03005140,
0x010051a4,
0x02005208,
0x0000523a,
0x0200526c,
0x0000529e,
0x030052d0,
0x00005302,
0x02005334,
0x00005398,
0x020053fc,
0x00005460,
0x020054c4,
0x00005528,
0x0300555a,
0x0000558c,
0x030055f0,
0x01005654,
0x030056b8,
0x0100571c,
0x0200574e,
0x00005780,
0x020057b2,
0x000057e4,
0x02005816,
0x00005848,
0x020058ac,
0x010058de,
0x03005910,
0x00005942,
0x02005974,
0x000059d8,
0x02005a3c,
0x00005aa0,
0x02005b04,
0x00005b68,
0x03005b9a,
0x01005bcc,
0x02005bfe,
0x00005c30,
0x02005c94,
0x00005cf8,
0x02005d5c,
0x00005dc0,
0x02005e24,
0x00005e88,
0x03005eba,
0x01005eec,
0x03005f1e,
0x00005f50,
0x02005fb4,
0x00006018,
0x0200607c,
0x000060e0,
0x02006144,
0x01006176,
0x030061a8,
0x000061da,
0x0200620c,
0x0100623e,
0x03006270,
0x000062a2,
0x020062d4,
0x00006338,
0x0200639c,
0x00006400,
0x02006464,
0x000064c8,
0x030064fa,
0x0000652c,
0x0300655e,
0x00006590,
0x030065c2,
0x010065f4,
0x03006658,
0x010066bc,
0x03006720,
0x01006784,
0x030067e8,
0x0000681a,
0x0200684c,
0x0100687e,
0x030068b0,
0x01006914,
0x03006978,
0x010069dc,
0x03006a40,
0x01006aa4,
0x03006b08,
0x00006b3a,
0x02006b6c,
0x00006b9e,
0x03006bd0,
0x01006c34,
0x03006c98,
0x01006cfc,
0x03006d60,
0x01006dc4,
0x02006df6,
0x00006e28,
0x03006e5a,
0x01006e8c,
0x02006ebe,
0x00006ef0,
0x03006f22,
0x01006f54,
0x03006fb8,
0x0100701c,
0x03007080,
0x010070e4,
0x03007148,
0x0000717a,
0x030071ac,
0x000071de,
0x03007210,
0x00007242,
0x02007274,
0x000072d8,
0x0200733c,
0x000073a0,
0x02007404,
0x00007468,
0x020074cc,
0x00007530,
0x03007562,
0x01007594,
0x020075c6,
0x010075f8,
0x0200762a,
0x0000765c,
0x0300768e,
0x000076c0,
0x030076f2,
0x01007724,
0x02007756,
0x01007788,
0x020077ba,
0x000077ec,
0x0300781e,
0x00007850,
0x03007882,
0x010078b4,
0x020078e6,
0x00007918,
0x0300794a,
0x0100797c,
0x020079ae,
0x000079e0,
0x03007a12,
0x01007a44,
0x03007a76,
0x00007aa8,
0x03007ada,
0x00007b0c,
0x03007b3e,
0x00007b70,
0x03007ba2,
0x01007bd4,
0x02007c9c,
0x01007dc8,
0x02007e2c,
0x00007e5e,
0x03007e90,
0x00007ec2,
0x02007ef4,
0x01007f58,
0x02007fbc,
0x00008084,
0x030080e8,
0x0000814c,
0x02008214,
0x01008278,
0x020082dc,
0x000083a4,
0x02008408,
0x0000846c,
0x020084d0,
0x00008502,
0x02008534,
0x01008598,
0x020085fc,
0x000086c4,
0x03008728,
0x0000878c,
0x02008854,
0x010088b8,
0x0200891c,
0x010089e4,
0x03008a16,
0x00008a48,
0x03008a7a,
0x01008aac,
0x03008ade,
0x00008b10,
0x03008b42,
0x01008b74,
0x02008bd8,
0x01008c3c,
0x03008d04,
0x00008d68,
0x03008dcc,
0x01008e94,
0x02008ef8,
0x01008f5c,
0x03009024,
0x01009056,
0x03009088,
0x010090ba,
0x030090ec,
0x0100911e,
0x03009150,
0x00009182,
0x020091b4,
0x01009218,
0x0200927c,
0x00009343,
0x030093a8,
0x0000940c,
0x020094d3,
0x01009537,
0x0200959c,
0x01009663,
0x03009696,
0x000096c7,
0x030096f9,
0x0100972b,
0x0200975d,
0x00009790,
0x030097c1,
0x000097f3,
0x03009857,
0x010098bb,
0x0200991f,
0x00009983,
0x030099e7,
0x01009a4b,
0x02009aaf,
0x00009b13,
0x03009b45,
0x00009b77,
0x03009ba9,
0x01009bdb,
0x02009c0d,
0x01009c3f,
0x02009c71,
0x00009ca3,
0x03009cd5,
0x00009d07,
0x03009d39,
0x01009d6b,
0x02009d9d,
0x01009dcf,
0x02009e01,
0x00009e33,
0x03009e65,
0x01009e97,
0x02009ec9,
0x00009efb,
0x03009f2d,
0x01009f5f,
0x02009f91,
0x00009fc3,
0x03009ff5,
0x0000a027,
0x0300a059,
0x0100a08b,
0x0200a0bd,
0x0100a0ef,
0x0200a121,
0x0100a153,
0x0300a2e3,
0x0100a473,
0x0300a53b,
0x0000a59f,
0x0300a603,
0x0000a6cb,
0x0200a793,
0x0100a85b,
0x0300a923,
0x0100a9eb,
0x0200aab3,
0x0000ab7b,
0x0300abdf,
0x0000ac43,
0x0300ad0b,
0x0100add3,
0x0300af63,
0x0100b0f3,
0x0300b1bb,
0x0000b21f,
0x0300b283,
0x0000b34b,
0x0200b413,
0x0100b4db,
0x0300b5a3,
0x0100b66b,
0x0300b733,
0x0000b797,
0x0200b7fb,
0x0000b85f,
0x0300b8c3,
0x0000b98b,
0x0200ba53,
0x0100bb1b,
0x0300bbe3,
0x0100bcab,
0x0200bd73,
0x0000be3b,
0x0300be9f,
0x0000bf03,
0x0300bfcb,
0x0000c093,
0x0200c0f7,
0x0100c15b,
0x0200c1bf,
0x0000c223,
0x0300c287,
0x0000c2eb,
0x0300c34f,
0x0000c3b3,
0x0200c417,
0x0100c47b,
0x0200c4df,
0x0000c543,
0x0300c5a7,
0x0100c60b,
0x0200c66f,
0x0000c6d3,
0x0300c737,
0x0000c79b,
0x0300c7ff,
0x0000c863,
0x0300c8c7,
0x0000c92b,
0x0300c98f,
0x0100c9f3,
0x0200ca57,
0x0000cabb,
0x0300cb1f,
0x0100cb83,
0x0200cbe7,
0x0000cc4b,
0x0300ccaf,
0x0100cd13,
0x0200cd77,
0x0000cddb,
0x0300ce3f,
0x0100cea3,
0x0300cf07,
0x0000cf6b,
0x0300cfcf,
0x0100d033,
0x0200d097,
0x0000d0fb,
0x0300d15f,
0x0100d1c3,
0x0200d227,
0x0000d28b,
0x0300d2ef,
0x0100d353,
0x0300d385,
0x0100d3b7,
0x0300d3e9,
0x0100d41b,
0x0300d44d,
0x0100d47f,
0x0300d4b1,
0x0100d4e3,
0x0300d547,
0x0000d5ab,
0x0300d60f,
0x0100d673,
0x0200d6d7,
0x0000d73b,
0x0300d79f,
0x0100d803,
0x0200d867,
0x0000d8cb,
0x0300d92f,
0x0100d993,
0x0200d9f7,
0x0000da5b,
0x0300dabf,
0x0100db23,
0x0300db87,
0x0000dbeb,
0x0300dc4f,
0x0100dcb3,
0x0300dd17,
0x0000dd7b,
0x0200dddf,
0x0000de43,
0x0200dea7,
0x0100df0b,
0x0300df6f,
0x0100dfd3,
0x0300e037,
0x0100e09b,
0x0300e0ff,
0x0000e163,
0x0200e195,
0x0000e1f9,
0x0300e22b,
0x0100e25d,
0x0300e28f,
0x0000e2c1,
0x0200e2f3,
0x0100e357,
0x0300e3bb,
0x0000e41f,
0x0200e483,
0x0100e4e7,
0x0300e54b,
0x0000e5af,
0x0200e613,
0x0100e677,
0x0300e6db,
0x0000e73f,
0x0200e7a3,
0x0000e807,
0x0300e86b,
0x0000e8cf,
0x0200e933,
0x0100e997,
0x0300e9fb,
0x0000ea5f,
0x0200eac3,
0x0100eb27,
0x0300eb8b,
0x0000ebef,
0x0200ec53,
0x0000ec85,
0x0200ecb7,
0x0000ece9,
0x0200ed1b,
0x0000ed4d,
0x0200ed7f,
0x0000edb1,
0x0200ede3,
0x0000ee47,
0x0300eeab,
0x0000ef0f,
0x0200ef73,
0x0100efd7,
0x0300f03b,
0x0000f09f,
0x0200f103,
0x0100f167,
0x0300f1cb,
0x0000f22f,
0x0200f293,
0x0100f2f7,
0x0300f35b,
0x0000f3bf,
0x0200f423,
0x0000f487,
0x0300f4eb,
0x0000f54f,
0x0200f5b3,
0x0000f617,
0x0300f67b,
0x0100f6df,
0x0300f743,
0x0100f7a7,
0x0200f80b,
0x0000f86f,
0x0200f8d3,
0x0000f937,
0x0200f99b,
0x0000f9ff,
0x0300fa63,
0x0100fa95,
0x0300faf9,
0x0000fb2b,
0x0200fb5d,
0x0000fb8f,
0x0300fbc1,
0x0100fbf3,
0x0300fc57,
0x0000fc89,
0x0300fcbb,
0x0000fced,
0x0300fd1f,
0x0000fd51,
0x0300fd83,
0x0000fdb5,
0x0300fde7,
0x0000fe19,
0x0300fe4b,
0x0000fe7d,
0x0300feaf,
0x0000fee1,
0x0200ff13,
0x0100ff45,
0x0200ff77,
0x0100ffa9,
0x0200ffdb,
0x0101000d,
0x0201003f,
0x01010071,
0x020100a3,
0x010100d5,
0x02010107,
0x01010139,
0x0201016b,
0x0101019d,
0x020101cf,
0x01010201,
0x03010233,
0x00010265,
0x03010297,
0x000102c9,
0x030102fb,
0x0001032d,
0x0301035f,
0x00010391,
0x020103c3,
0x010103f5,
0x02010427,
0x01010459,
0x0201048b,
0x010104bd,
0x020104ef,
0x01010521,
0x03010553,
0x00010585,
0x030105b7,
0x000105e9,
0x0301061b,
0x0001064d,
0x0301067f,
0x000106b1,
0x020106e3,
0x01010715,
0x02010747,
0x01010779,
0x020107ab,
0x010107dd,
0x0201080f,
0x01010841,
0x03010873,
0x000108a5,
0x030108d7,
0x00010909,
0x0201093b,
0x0101096d,
0x0201099f,
0x010109d1,
0x03010a03,
0x00010a35,
0x03010a67,
0x00010a99,
0x02010acb,
0x01010afd,
0x02010b2f,
0x01010b61,
0x03010b93,
0x01010bc5,
0x03010bf7,
0x01010c29,
0x03010c5b,
0x01010c8d,
0x03010cbf,
0x01010cf1,
0x03010d23,
0x01010d55,
0x03010d87,
0x01010db9,
0x03010deb,
0x01010e1d,
0x03010e4f,
0x01010e81,
0x03010eb3,
0x01010f17,
0x03010f7b,
0x01010fdf,
0x03011043,
0x000110a7,
0x0201110b,
0x0001116f,
0x030111d3,
0x01011237,
0x0301129b,
0x010112ff,
0x03011363,
0x010113c7,
0x0301142b,
0x0001148f,
0x030114f3,
0x01011557,
0x030115bb,
0x0101161f,
0x03011683,
0x000116e7,
0x0201174b,
0x000117af,
0x03011813,
0x01011877,
0x030118db,
0x0101190d,
0x0201193f,
0x01011971,
0x020119d5,
0x01011a07,
0x02011a39,
0x00011a6b,
0x02011a9d,
0x00011acf,
0x03011b33,
0x01011b97,
0x03011bfb,
0x01011c5f,
0x03011cc3,
0x00011d27,
0x02011d8b,
0x00011def,
0x03011e53,
0x01011eb7,
0x03011f1b,
0x01011f7f,
0x02011fe3,
0x00012047,
0x020120ab,
0x0001210f,
0x03012173,
0x000121d7,
0x0201223b,
0x0001229f,
0x02012303,
0x00012367,
0x020123cb,
0x0001242f,
0x03012493,
0x000124f7,
0x0201255b,
0x0101258d,
0x030125bf,
0x000125f1,
0x02012655,
0x00012687,
0x020126b9,
0x010126eb,
0x0201271d,
0x0001274f,
0x030127b3,
0x00012817,
0x0201287b,
0x000128df,
0x02012943,
0x000129a7,
0x02012a0b,
0x00012a6f,
0x02012ad3,
0x01012b37,
0x03012b9b,
0x00012bff,
0x03012c63,
0x01012cc7,
0x03012d2b,
0x01012d8f,
0x03012df3,
0x00012e57,
0x02012ebb,
0x00012f1f,
0x02012f83,
0x00012fe7,
0x0201304b,
0x000130af,
0x02013113,
0x01013177,
0x030131db,
0x0101320d,
0x0201323f,
0x01013271,
0x020132d5,
0x01013307,
0x02013339,
0x0001336b,
0x0201339d,
0x000133cf,
0x03013433,
0x01013497,
0x030134fb,
0x0101355f,
0x030135c3,
0x00013627,
0x0201368b,
0x000136ef,
0x02013753,
0x000137b7,
0x0201381b,
0x0001387f,
0x020138e3,
0x00013947,
0x030139ab,
0x01013a0f,
0x03013a73,
0x01013ad7,
0x03013b3b,
0x00013b9f,
0x02013c03,
0x00013c67,
0x02013ccb,
0x00013d2f,
0x02013d93,
0x01013dc5,
0x02013df7,
0x01013e29,
0x03013e5b,
0x00013e8d,
0x02013ebf,
0x01013ef1,
0x03013f23,
0x00013f87,
0x03013feb,
0x0001404f,
0x030140b3,
0x000140e5,
0x02014117,
0x0001417b,
0x030141ad,
0x010141df,
0x02014243,
0x01014275,
0x030142a7,
0x0001430b,
0x0301433d,
0x0101436f,
0x030143d3,
0x00014405,
0x02014437,
0x0001449b,
0x030144cd,
0x010144ff,
0x03014531,
0x00014563,
0x02014595,
0x000145c7,
0x030145f9,
0x0101462b,
0x0301465d,
0x0101468f,
0x030146c1,
0x010146f3,
0x020147bb,
0x01014883,
0x0201494b,
0x01014a13,
0x03014adb,
0x00014b3f,
0x02014ba3,
0x01014c6b,
0x03014ccf,
0x01014d33,
0x02014dfb,
0x00014e5f,
0x02014ec3,
0x01014ef5,
0x03014f27,
0x01014f59,
0x03014f8b,
0x01014fbd,
0x03014fef,
0x01015021,
0x02015053,
0x0101511b,
0x0301514d,
0x0101517f,
0x020151b1,
0x000151e3,
0x02015215,
0x01015247,
0x03015279,
0x010152ab,
0x03015373,
0x010153a5,
0x020153d7,
0x01015409,
0x0301543b,
0x0101546d,
0x0201549f,
0x010154d1,
0x03015503,
0x01015535,
0x02015567,
0x00015599,
0x020155cb,
0x010155fd,
0x0301562f,
0x01015661,
0x02015693,
0x000156c5,
0x020156f7,
0x01015729,
0x0301575b,
0x0101578d,
0x020157bf,
0x000157f1,
0x02015823,
0x00015855,
0x02015887,
0x000158b9,
0x020158eb,
0x0001591d,
0x0201594f,
0x00015981,
0x020159b3,
0x000159e5,
0x02015a17,
0x00015a49,
0x02015a7b,
0x00015aad,
0x02015adf,
0x00015b11,
0x02015b43,
0x01015c0b,
0x02015d37,
0x00015e63,
0x03015f2b,
0x00016057,
0x02016183,
0x0101624b,
0x02016377,
0x000164a3,
0x0301656b,
0x00016697,
0x020167c3,
0x0101688b,
0x020169b7,
0x00016ae3,
0x03016bab,
0x00016cd7,
0x02016e03,
0x01016ecb,
0x02016ff7,
0x00017123,
0x03017187,
0x010171eb,
0x0301724f,
0x000172b3,
0x02017317,
0x0001737b,
0x030173ad,
0x000173df,
0x03017411,
0x00017443,
0x030174a7,
0x0101750b,
0x0201756f,
0x000175d3,
0x03017637,
0x0101769b,
0x020176ff,
0x00017763,
0x030177c7,
0x0101782b,
0x0201788f,
0x000178f3,
0x02017957,
0x010179bb,
0x03017a1f,
0x00017a83,
0x03017ae7,
0x01017b4b,
0x02017baf,
0x00017c13,
0x03017c77,
0x01017cdb,
0x02017d3f,
0x00017da3,
0x03017e07,
0x01017e6b,
0x02017ecf,
0x01017f33,
0x03017f65,
0x00017f97,
0x03017fc9,
0x00017ffb,
0x0301802d,
0x0001805f,
0x03018091,
0x000180c3,
0x03018127,
0x0101818b,
0x020181ef,
0x00018253,
0x030182b7,
0x0101831b,
0x0201837f,
0x000183e3,
0x03018447,
0x010184ab,
0x0201850f,
0x00018573,
0x020185d7,
0x0001863b,
0x0201869f,
0x00018703,
0x03018767,
0x010187cb,
0x0201882f,
0x00018893,
0x030188f7,
0x0101895b,
0x020189bf,
0x00018a23,
0x03018a87,
0x01018aeb,
0x02018b4f,
0x00018bb3,
0x03018be5,
0x00018c17,
0x03018c49,
0x01018c7b,
0x02018cad,
0x00018cdf,
0x03018d11,
0x00018d43,
0x03018da7,
0x01018e0b,
0x02018e6f,
0x00018ed3,
0x02018f37,
0x01018f9b,
0x03018fff,
0x01019063,
0x030190c7,
0x000190f9,
0x0301912b,
0x0101918f,
0x030191f3,
0x00019257,
0x020192bb,
0x0001931f,
0x03019351,
0x01019383,
0x020193b5,
0x010193e7,
0x02019419,
0x0101944b,
0x0201947d,
0x010194af,
0x020194e1,
0x00019513,
0x03019545,
0x00019577,
0x030195a9,
0x000195db,
0x0301960d,
0x0001963f,
0x03019671,
0x010196a3,
0x02019707,
0x0001976b,
0x030197cf,
0x01019833,
0x03019897,
0x010198fb,
0x0201995f,
0x010199c3,
0x03019a27,
0x01019a8b,
0x02019aef,
0x00019b53,
0x02019bb7,
0x01019c1b,
0x02019c7f,
0x00019ce3,
0x03019d47,
0x01019dab,
0x03019e0f,
0x01019e73,
0x03019ed7,
0x01019f3b,
0x02019f9f,
0x0001a003,
0x0201a067,
0x0101a0cb,
0x0301a12f,
0x0101a193,
0x0201a1f7,
0x0001a25b,
0x0201a2bf,
0x0001a323,
0x0201a387,
0x0101a3eb,
0x0301a44f,
0x0101a4b3,
0x0201a517,
0x0001a57b,
0x0201a5df,
0x0101a643,
0x0301a6a7,
0x0101a70b,
0x0301a76f,
0x0101a7d3,
0x0301a837,
0x0101a89b,
0x0301a8ff,
0x0101a963,
0x0301a9c7,
0x0101aa2b,
0x0301aaf3,
0x0101ab57,
0x0201abbb,
0x0001ac1f,
0x0201ac83,
0x0001ad4b,
0x0201adaf,
0x0101ae13,
0x0301aedb,
0x0101af3f,
0x0201afa3,
0x0001b06b,
0x0201b09d,
0x0101b0cf,
0x0201b101,
0x0101b133,
0x0301b197,
0x0101b1fb,
0x0301b25f,
0x0101b2c3,
0x0301b327,
0x0101b38b,
0x0301b3ef,
0x0101b453,
0x0301b4b7,
0x0101b51b,
0x0301b57f,
0x0101b5b1,
0x0201b5e3,
0x0101b615,
0x0201b647,
0x0001b6ab,
0x0201b70f,
0x0101b773,
0x0301b7d7,
0x0101b83b,
0x0301b89f,
0x0101b903,
0x0201b967,
0x0001b9cb,
0x0201ba2f,
0x0101ba93,
0x0301baf7,
0x0001bb29,
0x0301bb5b,
0x0001bb8d,
0x0301bbbf,
0x0001bbf1,
0x0201bc23,
0x0101bc87,
0x0201bceb,
0x0101be17,
0x0201be7b,
0x0101bedf,
0x0201bfa7,
0x0101c00b,
0x0201c06f,
0x0001c0d3,
0x0201c105,
0x0001c137,
0x0201c169,
0x0001c19b,
0x0201c1cd,
0x0001c1ff,
0x0201c231,
0x0101c263,
0x0201c2c7,
0x0101c32b,
0x0201c457,
0x0101c51f,
0x0301c583,
0x0001c5e7,
0x0301c64b,
0x0001c6af,
0x0201c713,
0x0001c777,
0x0301c7a9,
0x0001c7db,
0x0301c80d,
0x0001c83f,
0x0301c871,
0x0101c8a3,
0x0201c907,
0x0101c96b,
0x0201ca97,
0x0101cafb,
0x0201cb5f,
0x0101cc27,
0x0201cc8b,
0x0101ccef,
0x0301cd53,
0x0101cd85,
0x0301cdb7,
0x0101cde9,
0x0301ce1b,
0x0101ce4d,
0x0301ce7f,
0x0101ceb1,
0x0201cee3,
0x0101cf47,
0x0201cfab,
0x0101d0d7,
0x0201d19f,
0x0101d203,
0x0201d235,
0x0101d267,
0x0201d299,
0x0101d2cb,
0x0201d2fd,
0x0001d32f,
0x0201d361,
0x0001d393,
0x0201d3c5,
0x0001d3f7,
0x0201d429,
0x0001d45b,
0x0201d48d,
0x0001d4bf,
0x0201d4f1,
};