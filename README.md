# DNS-based-DDoS-mitigation
Puzzle-centric DDoS Traffic Control: A DNS-based Approach

``` Unicode
DNS-based-DDoS-mitigation (expected)
  │ 
  ├── application
  │    │     
  │    ├── include
  │    │    ├── puzzle.h: structures and functions about hash puzzle
  │    │    ├── circular_buffer.h
  │    │    ├── tcp_syn_monitor.h
  │    │    ├── monitor_log.h
  │    │    ├── detect_ddos.h
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
  └── rbpi-kernel: link to modified linux kernel source code for raspberry pi
```
