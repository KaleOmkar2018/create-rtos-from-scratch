#ifndef SVCEXCEPTIONS_H
#define SVCEXCEPTIONS_H

int __svc(0x00) svc_service_add(int x, int y);
int __svc(0x01) svc_service_sub(int x, int y);
int __svc(0x02) svc_service_mul(int x, int y);
int __svc(0x03) svc_service_div(int x, int y);

void SVC_Handler_C(unsigned int* svc_args);

#endif
