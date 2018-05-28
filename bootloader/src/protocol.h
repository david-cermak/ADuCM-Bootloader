void start();

int receive(char* ch, int timeout);

enum e_process_command process_command(int bytes, int command, uint8_t* data);
