//------------------------------------------------ ----------------------
// https://github.com/lovyan03/LovyanGFX/blob/master/examples/HowToUse/2_user_setting/2_user_setting.ino
class LGFX: public lgfx::LGFX_Device{
   lgfx::Panel_ILI9341 _panel_instance;
   lgfx::Bus_SPI _bus_instance;
   lgfx::Light_PWM _light_instance;
   lgfx::Touch_XPT2046 _touch_instance;
//------------------------------------------------ ----------------------
public:LGFX(void){
   { // Configura le impostazioni di controllo del bus.
   auto cfg = _bus_instance.config();// Ottieni la struttura di configurazione del bus.
                                //Impostazioni del bus SPI
   cfg.spi_host = SPI2_HOST; // Seleziona la SPI da utilizzare (VSPI_HOST o HSPI_HOST)
   cfg.spi_mode = 0; // Imposta la modalità di comunicazione SPI (0 ~ 3)
   cfg.freq_write = 40000000; // Orologio SPI durante la trasmissione (massimo 80 MHz, arrotondando 80 MHz a un numero intero)
   cfg.freq_read = 16000000; // Orologio SPI durante la ricezione
   cfg.spi_3wire = false; // Imposta true se la ricezione viene eseguita utilizzando il pin MOSI
   cfg.use_lock = true; // Imposta true per utilizzare il blocco delle transazioni
   cfg.dma_channel= 1; // Impostazione canale DMA (1or2,0=disabilita) (0=DMA non utilizzato)
   cfg.pin_sclk = 14; // Imposta il numero pin SPI SCLK SCK
   cfg.pin_mosi = 13; // Imposta il numero pin SPI MOSI SDI
   cfg.pin_miso = 12; // Imposta il numero pin SPI MISO (-1 = disabilita) SDO
   cfg.pin_dc = 2; // Imposta il numero pin SPI D/C (-1 = disabilita) RS
   // Se usi lo stesso bus SPI della scheda SD, assicurati di impostare MISO senza ometterlo.
   _bus_instance.config(cfg); // Applica le impostazioni al bus.
   _panel_instance.setBus(&_bus_instance);// Imposta il bus sul pannello.
   }
   { // Imposta il controllo del pannello di visualizzazione.
   auto cfg = _panel_instance.config();// Ottieni la struttura per le impostazioni del pannello di visualizzazione.
   cfg.pin_cs = 15; // Numero pin a cui è collegato CS (-1 = disabilitato)
   cfg.pin_rst = -1; // Numero pin a cui è collegato RST (-1 = disabilitato)
   cfg.pin_busy = -1; // Numero pin a cui è collegato BUSY (-1 = disabilitato)
   cfg.memory_width = 240; // Larghezza massima supportata dall'IC del driver
   cfg.memory_height = 320; // Altezza massima supportata dall'IC del driver
   cfg.panel_width = 240; // Larghezza effettiva visualizzabile
   cfg.panel_height = 320; // Altezza effettiva del display
   cfg.offset_x = 0; // Quantità di offset della direzione X del pannello
   cfg.offset_y = 0; // Quantità di offset della direzione Y del pannello
   cfg.offset_rotation = 0; // Offset del valore della direzione di rotazione 0~7 (4~7 sono capovolti)
   cfg.dummy_read_pixel= 8; // Numero di bit per la lettura fittizia prima della lettura dei pixel
   cfg.dummy_read_bits = 1; // Numero di bit per la lettura fittizia prima di leggere i dati all'esterno del pixel
   cfg.readable = true; // Imposta su true se i dati possono essere letti
   cfg.invert = false; // Imposta su true se la luminosità del pannello è invertita
   cfg.rgb_order = false; // Imposta su true se il rosso e il blu sul pannello vengono scambiati, ok
   cfg.dlen_16bit = false; // Pannello che invia i dati in unità di 16 bit Impostato su true
   cfg.bus_shared = false; // Condividi il bus con la scheda SD Impostato su true
   _panel_instance.config(cfg);
   }
   { // Imposta il controllo della retroilluminazione. (Cancellare se non necessario)
   auto cfg = _light_instance.config();// Ottieni la struttura per la configurazione della retroilluminazione.
   cfg.pin_bl = 21; // Numero del pin BL a cui è collegata la retroilluminazione
   cfg.invert = false; // true per invertire la luminosità della retroilluminazione
   cfg.freq = 44100; // Frequenza PWM della retroilluminazione
   cfg.pwm_channel = 7; // Numero del canale PWM da utilizzare
   _light_instance.config(cfg);
   _panel_instance.setLight(&_light_instance);//Imposta la retroilluminazione sul pannello.
   }
   { // Configura il controllo touchscreen. (Cancellare se non necessario)
   auto cfg  = _touch_instance.config();
   cfg.x_min = 300; // Valore X minimo ottenuto dal touch screen (valore grezzo)
   cfg.x_max = 3900; // Valore X massimo ottenuto dal touch screen (valore grezzo)
   cfg.y_min = 200; // Valore Y minimo ottenuto dal touch screen (valore grezzo)
   cfg.y_max = 3700; // Valore Y massimo ottenuto dal touch screen (valore grezzo)
   cfg.pin_int = 36; // Numero pin a cui è collegato INT, TP IRQ
   cfg.bus_shared = false; // Imposta true se si utilizza lo stesso bus dello schermo
   cfg.offset_rotation = 6; // Regolazione quando la direzione del display e del tocco non corrispondono Impostare con un valore compreso tra 0 e 7
   // Per la connessione SPI
   cfg.spi_host = VSPI_HOST;// Seleziona la SPI da utilizzare (HSPI_HOST o VSPI_HOST)
   cfg.freq = 1000000; // Imposta l'orologio SPI
   cfg.pin_sclk = 25; // Numero pin a cui è collegato SCLK, TP CLK
   cfg.pin_mosi = 32; // Numero del pin a cui è collegato MOSI, TP DIN
   cfg.pin_miso = 39; // Numero pin a cui è collegato MISO, TP DOUT
   cfg.pin_cs = 33; // Numero pin a cui è collegato CS, TP CS
   _touch_instance.config(cfg);
   _panel_instance.setTouch(&_touch_instance); // Imposta il touch screen sul pannello.
   }
   setPanel(&_panel_instance);// Imposta il pannello da utilizzare.
   }
};
LGFX tft; // Crea un'istanza della classe preparata.

//================================================ = =====================