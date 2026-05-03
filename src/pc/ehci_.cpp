#include <stdint.h>
#include "../utils/util.h"
#include "../cpu/pci.h"
#include "../mem/paging.h"
#include "../pc/keyboard.h"
#include "timer.h"
#include "Disk.h"

extern "C" Device USBs[64];
extern "C" u32 USBsnum;
volatile u32 EHCI_BASE = USBs[0].bases[0];

#define EHCI_RUN       (1 << 0)
#define EHCI_ASYNC_EN  (1 << 5)
#define EHCI_HC_HALT   (1 << 12)

typedef volatile struct{

}EHCI_Cap;


typedef volatile struct {
    uint32_t UsbCmd;
    uint32_t UsbSts;
    uint32_t UsbIntr;
    uint32_t FrameIndex;
    uint32_t SegmentSelector;
    uint32_t FrameListBase;
    uint32_t AsyncListAddr;
    uint8_t  Reserved[36];
    uint32_t ConfigFlag;
    uint32_t PortSC[8];
} EHCI_OpRegs;

typedef volatile struct {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} __attribute__((packed)) USBSetup;

typedef volatile struct {
    uint32_t NextTD;
    uint32_t AltNextTD;
    /*struct {
        uint32_t PID : 2;
        uint32_t ErrorCounter : 2;
        uint32_t CurrentPage : 3;
        uint32_t InterruptOnComplete : 1;
        uint32_t BytesToTransfer : 15;
        uint32_t DataToggle : 1;
        uint32_t Active : 1;
        uint32_t Halt : 1;
        uint32_t BufferError : 1;
        uint32_t BabbleDetected : 1;
        uint32_t TransactionError : 1;
        uint32_t MissedMicroFrame : 1;
        uint32_t SplitXState : 1;
        uint32_t PingState : 1;
        uint32_t Reserved : 1;
    } token;*/
    u32 token;

    uint32_t Buffer1;
    uint32_t Buffer2;
    uint32_t Buffer3;
    uint32_t Buffer4;
    uint32_t Buffer5;

    uint32_t BufferHi1;
    uint32_t BufferHi2;
    uint32_t BufferHi3;
    uint32_t BufferHi4;
    uint32_t BufferHi5;
} __attribute__((packed, aligned(64))) EHCI_QTD;

typedef volatile struct {
    uint32_t HorizLinkPtr;
    /*struct {
        uint32_t DeviceAddress : 7;
        uint32_t InactiveOnNext : 1;
        uint32_t EndpointNumber : 4;
        uint32_t EndpointSpeed : 2;
        uint32_t DataToggleControl : 1;
        uint32_t HeadOfReclimationList : 1;
        uint32_t MaxPacketLength : 11;
        uint32_t ControlEndpointFlag : 1;
        uint32_t NakReloadCount : 4;
    } epchar;*/
     u32 epchar;
    /*struct {
        uint32_t InterruptScheduleMask : 8;
        uint32_t SplitCompletionMask : 8;
        uint32_t HubAddr : 7;
        uint32_t PortNumber : 7;
        uint32_t Mult : 2;
    } epcap;*/
    u32 epcap;
    uint32_t CurrentTD;
    uint32_t reserved[4];
    u8 pnd[256];
    //u8 padding[256-16-sizeof(EHCI_QH)];
} __attribute__((packed, aligned(64))) EHCI_QH;
u32 i = sizeof(EHCI_QH);
static volatile EHCI_QH qh __attribute__((aligned(64)));
static volatile EHCI_QTD qtd_setup __attribute__((aligned(64)));
static volatile EHCI_QTD qtd_in    __attribute__((aligned(64)));
static volatile EHCI_QTD qtd_out   __attribute__((aligned(64)));
static volatile USBSetup setup __attribute__((aligned(8))) = {
    .bmRequestType = 0x80,
    .bRequest = 0x06,
    .wValue = 0x0100,
    .wIndex = 0x0000,
    .wLength = 8
};
static volatile uint8_t buffer[18] __attribute__((aligned(8)));

void USBInit() {
    EHCI_BASE = USBs[0].bases[0];
    link_page((u32)malloc(0x1000), EHCI_BASE);
    EHCI_OpRegs* ehci = (EHCI_OpRegs*)(EHCI_BASE + 0x20);

    printc("\nEHCI MMIO at: ", 10); printc(Hex(EHCI_BASE), 12);
    ehci->UsbCmd|=(8 << 16);
    sleep(200);
    ehci->UsbCmd|=(1<<1)|(8 << 16);
    sleep(200);
    ehci->FrameIndex=0;
    ehci->SegmentSelector=0;
    ehci->UsbCmd|=1;
    //ehci->UsbCmd&=~2;
    // إعادة ضبط المنفذ
    ehci->PortSC[0] |= (1 << 8);
    sleep(10);
    ehci->PortSC[0] &= ~(1 << 8);
    sleep(50);
    
    // تحقق من الاتصال
    if (ehci->PortSC[0] & 0x1) {
        printc("Device connected!\n", 12);
    }

    // إيقاف وحدة EHCI مؤقتاً
    //ehci->UsbCmd &= ~EHCI_ASYNC_EN;
    //while (ehci->UsbSts & EHCI_HC_HALT);

    // 🌐 إعداد الـ QTD (SETUP)
    for (int i = 0; i < 5; i++) {
        ((u32*)&qtd_setup.Buffer1)[i] = 0;
        ((u32*)&qtd_setup.BufferHi1)[i] = 0;
    }
    /*qtd_setup.NextTD = (u32)physic_adress((u32)&qtd_in);
    qtd_setup.AltNextTD = 1;
    qtd_setup.token.PID = 0x2;  // SETUP
    qtd_setup.token.BytesToTransfer = 8;
    qtd_setup.token.DataToggle = 0;
    qtd_setup.token.Active = 1;
    qtd_setup.token.InterruptOnComplete = 1;
    qtd_setup.Buffer1 = (u32)physic_adress((u32)&setup);

    // 🌐 إعداد QTD للقراءة (IN)
    for (int i = 0; i < 5; i++) {
       ((u32*)&qtd_setup.Buffer1)[i] = 0;
       ((u32*)&qtd_setup.BufferHi1)[i] = 0;
    }
    qtd_in.NextTD = (u32)physic_adress((u32)&qtd_out);
    qtd_in.AltNextTD = 1;
    qtd_in.token.PID = 0x1; // IN
    qtd_in.token.BytesToTransfer = 8;
    qtd_in.token.DataToggle = 1;
    qtd_in.token.Active = 1;
    qtd_in.token.InterruptOnComplete = 1;
    qtd_in.Buffer1 = (u32)physic_adress((u32)&buffer);

    // 🌐 إعداد QTD OUT لإنهاء المعاملة
    for (int i = 0; i < 5; i++) {
        ((u32*)&qtd_setup.Buffer1)[i] = 0;
        ((u32*)&qtd_setup.BufferHi1)[i] = 0;
    }
    qtd_out.NextTD = 1;
    qtd_out.AltNextTD = 1;
    qtd_out.token.PID = 0x0; // OUT
    qtd_out.token.BytesToTransfer = 0;
    qtd_out.token.DataToggle = 1;
    qtd_out.token.Active = 1;
    qtd_out.token.InterruptOnComplete = 1;

    // ⚙️ تهيئة QH وربطه بـ QTD
    qh.HorizLinkPtr = 0x00000001;  // نهاية السلسلة
    qh.epchar.DeviceAddress = 0;
    qh.epchar.EndpointNumber = 0;
    qh.epchar.EndpointSpeed = 2;
    qh.epchar.MaxPacketLength = 64;
    qh.epchar.HeadOfReclimationList = 1;
    qh.epcap.Mult = 1;
    qh.CurrentTD = (u32)physic_adress((u32)&qtd_setup);
    qh.HorizLinkPtr = 0x00000001;
    // ربط QH بالتحكم
    ehci->AsyncListAddr = (u32)physic_adress((u32)&qh) & ~0x1F;

    // تشغيل EHCI
    ehci->ConfigFlag = 1;
    ehci->UsbCmd |= EHCI_ASYNC_EN;
    ehci->UsbCmd |= EHCI_RUN;

    // انتظار انتهاء آخر QTD
   /* while (qtd_out.token.Active) {
        sleep(1);
    }*/

    // طباعة النتيجة
    printc("USBCMD",14);printc(Bin(ehci->UsbCmd),12); printc("\n", 10);
    for (int i = 0; i < 8; i++) {
        printc("Byte ", 14); printc(Hex(i), 14); printc(": ", 14); printc(Hex(buffer[i]), 12); printc("\n", 10);
        
    }
    while(!key);
}
