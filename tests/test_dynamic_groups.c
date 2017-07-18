#include <stdio.h>
#include <stdlib.h>
#include "rtrlib/rtrlib.h"

int main(){
    //create a TCP transport socket
    struct tr_socket tr_tcp;
    char tcp_host[] = "rpki-validator.realmv6.org";
    char tcp_port[] = "8282";

    struct tr_tcp_config tcp_config = {
    tcp_host, //IP
    tcp_port, //Port
    NULL      //Source address
    };
    tr_tcp_init(&tcp_config, &tr_tcp);
    struct rtr_socket rtr_tcp;
    rtr_tcp.tr_socket = &tr_tcp;
    struct rtr_mgr_group groups[1];

    //The first group contains both TCP RTR sockets
    groups[0].sockets = malloc(sizeof(struct rtr_socket*));
    groups[0].sockets_len = 1;
    groups[0].sockets[0] = &rtr_tcp;
    groups[0].preference = 1;

    struct rtr_mgr_config_ll *conf;
    int ret = rtr_mgr_init(&conf, groups, 1, 30, 600, 600, NULL, NULL, NULL, NULL);

    //start the connection manager
    rtr_mgr_start(conf);

    //wait till at least one rtr_mgr_group is fully synchronized with the server
    while(!rtr_mgr_conf_in_sync(conf)) {
        sleep(1);
    }
    //
    ////validate the BGP-Route 10.10.0.0/24, origin ASN: 12345
    //struct lrtr_ip_addr pref;
    //lrtr_ip_str_to_addr("10.10.0.0", &pref);
    //enum pfxv_state result;
    //const uint8_t mask = 24;
    //rtr_mgr_validate(conf, 12345, &pref, mask, &result);
    //
    ////output the result of the prefix validation above
    ////to showcase the returned states.
    //char buffer[INET_ADDRSTRLEN];
    //lrtr_ip_addr_to_str(&pref, buffer, sizeof(buffer));

    //printf("RESULT: The prefix %s/%i ", buffer, mask);
    //switch(result) {
    //    case BGP_PFXV_STATE_VALID:
    //        printf("is valid.\n");
    //        break;
    //    case BGP_PFXV_STATE_INVALID:
    //        printf("is invalid.\n");
    //        break;
    //    case BGP_PFXV_STATE_NOT_FOUND:
    //        printf("was not found.\n");
    //        break;
    //    default:
    //        break;
    //}

    //rtr_mgr_stop(conf);
    //rtr_mgr_free(conf);
    //free(groups[0].sockets);
    //free(groups[1].sockets);
}