#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug )
  ,the_window_size(50)
  ,hist_rtt(30)
  ,min_rtt(100)
{}

/* Get current window size, in datagrams */
unsigned int Controller::window_size( void )
{
  /* Default: fixed window size of 100 outstanding datagrams */

  // Need to override default
  //unsigned int the_window_size = 50;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return the_window_size;
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp )
                                    /* in milliseconds */
{
  /* Default: take no action */

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << endl;
  }
}


/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  /* Default: take no action */

  double rtt =  timestamp_ack_received - send_timestamp_acked;
  // update min rtt
  if ( rtt < min_rtt )
    min_rtt = rtt;

  // Exponential Smoothing
  double discounting_factor = 0.95;
  double accumulate_rtt = discounting_factor*rtt + ( 1.0 - discounting_factor ) * hist_rtt;
  hist_rtt = accumulate_rtt;
  
  // Estimate queue length
  double est_queue = accumulate_rtt - min_rtt;
  

  if ( est_queue < 0.3 * min_rtt ){
    // safe, increase slowly
    the_window_size += 0.25;
    if (est_queue < 0.1 * min_rtt){
      // very safe, increase faster
      the_window_size += 0.5;
    }
  }
  else
    if (est_queue > 3 * min_rtt){
      // very long queue, decrease fast
      the_window_size *= 0.5;
    }
    else
      // decrease slowly
      the_window_size -= 0.25;

  if ( the_window_size < 1 )
    the_window_size = 1;

  if ( the_window_size > 200 )
    the_window_size = 200;
  
  //std::cout << "rtt, window_size, accumulate_rtt, min_rtt" << endl;
  //std::cout << rtt << "," << the_window_size << "," << accumulate_rtt << "," << min_rtt << endl;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  // change timeout to p*min_rtt
  //return 1000; /* timeout of one second */
  return 0.75*min_rtt;
}

