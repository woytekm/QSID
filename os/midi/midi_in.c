
void MIDI_IN_thread(void)
 {

   pollrc,end=0;
   ssize_t rc;
   char buff[1024];
   struct pollfd fds[1];
 
   G_MIDI_fd = MIDI_init();

   if( G_MIDI_fd == -1 )
    {
      printf("mididump: cannot initialize MIDI interface!\n");
      exit(-1);
    }

   bzero(buff,1024);

   fds[0].fd = G_MIDI_fd;
   fds[0].events = POLLIN;

   while(!end)
    {

      pollrc = poll( fds, 1, -1);

     if (pollrc < 0)
     {
       perror("poll");
     }

    else if( pollrc > 0)
     {
      if( fds[0].revents & POLLIN )
       {

         rc = read(MIDI_fd, buff, sizeof(buff) );

         if (rc > 0)
          {
           MIDI_dispatch(buff,rc);
           bzero(buff, rc);
          }

        }  


 }

