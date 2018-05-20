void start();

int receive(char* ch, int timeout);

int process_command(int bytes, int command, uint8_t* data);
