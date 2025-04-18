#ifndef _CommunicationPC_LIB
#define _CommunicationPC_LIB
#include <Arduino.h>
#include <Commun.h>
#include <BluetoothSerial.h>

#define ID_ACK_GENERAL                0xC0 //Ack pour tous le reste
#define ID_REPEAT_REQUEST             0xD0

#define ID_CMD_MOVE                   0xA0 //On reçoit le mouvement x y z à faire en millimetres
#define ID_ACK_CMD_MOVE               0xA1 //On accuse la réception
#define ID_SERVO_GRAB                 0xA2 //On demande de serrer la pince
#define ID_ACK_SERVO_GRAB             0xA3 //On accuse la réception
#define ID_SERVO_RELEASE              0xA4 //On demande de desserrer la pince
#define ID_ACK_SERVO_RELEASE          0xA5 //On accuse la réception
#define ID_HOMING                     0xA6 //On demande de faire le homing
#define ID_ACK_HOMING                 0xA7 //On accuse la réception
#define ID_SEND_CURRENT_POSITION      0xA8 //On nous demande d'envoyer la position courante
#define ID_ACK_SEND_CURRENT_POSITION  0xA9 //On accuse la réception en envoyant la position courante

#ifndef SIZE_FIFO
  #define SIZE_FIFO 32 //maximum 150 du fait du type char
  typedef struct Message{
    uint8_t id;
    uint8_t len;
    uint8_t *data;
    uint8_t checksum;
    // uint8_t checksum;
  }Message;
#endif

class OnReceive
{
private:
  static Message rxMsg[SIZE_FIFO]; 
  static int static_FIFO_ecriture;
  int FIFO_ecriture;
  // État de la réception 
  enum StateRx{
    WAITING_HEADER,
    RECEIVING_ID,
    RECEIVING_LEN,
    RECEIVING_DATA,
    RECEIVING_CHECKSUM,
    WAITING_FOOTER
  };
  StateRx currentState = WAITING_HEADER;
  uint8_t dataCounter = 0, byte;

public:
  OnReceive();

  void onReceive(uint8_t byte);

  static int getFIFO_ecriture(){
    return static_FIFO_ecriture;
  }

  static Message getMsg(int index){
    return rxMsg[index];
  }

  static void printMessage(Message msg);
};


class CommunicationPC
{
public:
    CommunicationPC(/* args */);

    void begin(HardwareSerial *srl = &Serial, long baud = 921600, String nameBT = "Robot_Turc_Echec");
    void end();

    void RxManage();

    void sendMsg(Message &txMsg);
    void sendMsg(uint8_t id, uint8_t len, uint8_t *data);
    void sendMsg(uint8_t id);
    void sendMsg(uint8_t id, uint8_t octet);
    void sendMsg(uint8_t id, uint8_t octet1, uint8_t octet2);
    void sendMsg(uint8_t id, uint8_t octet1, uint8_t octet2, uint8_t octet3);
    void sendMsg(uint8_t id, uint16_t nb);
    void sendMsg(uint8_t id, uint16_t nb, uint16_t nb2);
    void sendMsg(uint8_t id, uint16_t nb, uint16_t nb2, uint16_t nb3);
    void sendMsg(uint8_t id, uint32_t nb);
    void sendMsg(uint8_t id, uint32_t nb, uint32_t nb2);
    void sendMsg(uint8_t id, uint32_t nb, uint32_t nb2, uint32_t nb3);
    void sendMsg(uint8_t id, int16_t nb);
    void sendMsg(uint8_t id, int16_t nb, int16_t nb2);
    void sendMsg(uint8_t id, int16_t nb, int16_t nb2, int16_t nb3);

    void sendMsg(uint8_t id, Position *pos);

    void printMessage(Message msg);


    bool newMoveReceived();
    MOVE *getMove();

    void attachPosition(Position *pos);

private:
    HardwareSerial *_serial;
    BluetoothSerial *_serialBT;

    OnReceive com;
    OnReceive comBT;

    void onReceiveFunction(void);

    static CommunicationPC* instance;  // Pointer to the current instance
    static void onReceiveFunctionBTStatic(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
    void onReceiveFunctionBT(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

    MOVE move[SIZE_FIFO];
    int cursor_move_write = 0;
    int cursor_move_read = 0;
    
    Position *current_position;
};




#endif // _CommunicationPC_LIB