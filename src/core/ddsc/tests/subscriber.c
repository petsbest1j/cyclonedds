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
#include "dds/dds.h"

#include <stdio.h>
#include "CUnit/Test.h"

/* We are deliberately testing some bad arguments that SAL will complain about.
 * So, silence SAL regarding these issues. */
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 6387 28020)
#endif

static void on_data_available(dds_entity_t reader, void* arg)
{
  (void)reader;
  (void)arg;
}

static void on_publication_matched(dds_entity_t writer, const dds_publication_matched_status_t status, void* arg)
{
  (void)writer;
  (void)status;
  (void)arg;
}

CU_Test(ddsc_subscriber, notify_readers) {
  dds_entity_t participant;
  dds_entity_t subscriber;
  dds_return_t ret;

  participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
  CU_ASSERT_FATAL(participant > 0);


  subscriber = dds_create_subscriber(participant, NULL, NULL);
  CU_ASSERT_FATAL(subscriber > 0);

  /* todo implement tests */
  ret = dds_notify_readers(subscriber);
  CU_ASSERT_EQUAL_FATAL(dds_err_nr(ret), DDS_RETCODE_UNSUPPORTED);

  dds_delete(subscriber);
  dds_delete(participant);
}

CU_Test(ddsc_subscriber, create) {

  dds_entity_t participant;
  dds_entity_t subscriber;
  dds_listener_t *listener;
  dds_qos_t *sqos;

  participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
  CU_ASSERT_FATAL(participant > 0);

  /*** Verify participant parameter ***/

  subscriber = dds_create_subscriber(0, NULL, NULL);
  CU_ASSERT_EQUAL_FATAL(dds_err_nr(subscriber), DDS_RETCODE_BAD_PARAMETER);

  subscriber = dds_create_subscriber(participant, NULL, NULL);
  CU_ASSERT_FATAL(subscriber > 0);
  dds_delete(subscriber);

  /*** Verify qos parameter ***/

  sqos = dds_create_qos(); /* Use defaults (no user-defined policies) */
  subscriber = dds_create_subscriber(participant, sqos, NULL);
  CU_ASSERT_FATAL(subscriber > 0);
  dds_delete(subscriber);
  dds_delete_qos(sqos);

  sqos = dds_create_qos();
  dds_qset_destination_order(sqos, 3); /* Set invalid dest. order (ignored, not applicable for subscriber) */
  subscriber = dds_create_subscriber(participant, sqos, NULL);
  CU_ASSERT_FATAL(subscriber > 0);
  dds_delete(subscriber);
  dds_delete_qos(sqos);

  sqos = dds_create_qos();
  dds_qset_presentation(sqos, 123, 1, 1); /* Set invalid presentation policy */
  subscriber = dds_create_subscriber(participant, sqos, NULL);
  CU_ASSERT_EQUAL_FATAL(dds_err_nr(subscriber), DDS_RETCODE_INCONSISTENT_POLICY);
  dds_delete_qos(sqos);

  /*** Verify listener parameter ***/

  listener = dds_create_listener(NULL); /* Use defaults (all listeners unset) */
  subscriber = dds_create_subscriber(participant, NULL, listener);
  CU_ASSERT_FATAL(subscriber > 0);
  dds_delete(subscriber);
  dds_delete_listener(listener);

  listener = dds_create_listener(NULL);
  dds_lset_data_available(listener, &on_data_available); /* Set on_data_available listener */
  subscriber = dds_create_subscriber(participant, NULL, listener);
  CU_ASSERT_FATAL(subscriber > 0);
  dds_delete(subscriber);
  dds_delete_listener(listener);

  listener = dds_create_listener(NULL);
  dds_lset_publication_matched(listener, &on_publication_matched); /* Set on_publication_matched listener (ignored, not applicable for subscriber) */
  subscriber = dds_create_subscriber(participant, NULL, listener);
  CU_ASSERT_FATAL(subscriber > 0);
  dds_delete(subscriber);
  dds_delete_listener(listener);

  dds_delete(participant);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
