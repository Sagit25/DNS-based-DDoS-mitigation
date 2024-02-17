# DNS-based-DDoS-mitigation
DNS-based-DDoS-mitigation

``` Unicode
DNS-based-DDoS-mitigation (expected)
  │ 
  ├── application
  │    │     
  │    ├── include
  │    │    ├── puzzle.h: structures and functions about hash puzzle
  │    │    ├── circular_buffer.h
  │    │    ├── tcp_syn_monitor.h
  │    │    └── packet_handler.h
  │    │      
  │    └── src
  │         ├── auth.c: authoritative server
  │         ├── local.c: local DNS resolver
  │         ├── host.c: target server
  │         ├── client.c: benign client
  │         ├── bot.c: bot (don't solve hash puzzle)
  │         ├── monitor.c: monitor server
  │         └── monitor
  │         
  └── kernel(rbpi): whole modified linux kernel code for raspberry pi
```