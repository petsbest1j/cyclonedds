/*
 * Copyright(c) 2006 to 2018 ADLINK Technology Limited and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#ifndef Q_GC_H
#define Q_GC_H

#include "ddsi/q_thread.h"

#if defined (__cplusplus)
extern "C" {
#endif

struct gcreq;
struct gcreq_queue;

struct writer;
struct reader;
struct proxy_writer;
struct proxy_reader;

typedef void (*gcreq_cb_t) (struct gcreq *gcreq);

struct idx_vtime {
  unsigned idx;
  vtime_t vtime;
};

struct gcreq {
  struct gcreq *next;
  struct gcreq_queue *queue;
  gcreq_cb_t cb;
  void *arg;
  unsigned nvtimes;
  struct idx_vtime vtimes[1 /* really a flex ary */];
};

struct gcreq_queue *gcreq_queue_new (void);
void gcreq_queue_drain (struct gcreq_queue *q);
void gcreq_queue_free (struct gcreq_queue *q);

struct gcreq *gcreq_new (struct gcreq_queue *gcreq_queue, gcreq_cb_t cb);
void gcreq_free (struct gcreq *gcreq);
void gcreq_enqueue (struct gcreq *gcreq);
int gcreq_requeue (struct gcreq *gcreq, gcreq_cb_t cb);

#if defined (__cplusplus)
}
#endif

#endif /* Q_GC_H */
