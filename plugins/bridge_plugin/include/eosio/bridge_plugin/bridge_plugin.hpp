#pragma once
#include <appbase/application.hpp>
#include <eosio/chain_plugin/chain_plugin.hpp>
#include <eosio/chain/producer_schedule.hpp>
#include <eosio/chain/types.hpp>

namespace eosio {

using namespace appbase;
using namespace chain;

class bridge_plugin : public appbase::plugin<bridge_plugin> {
public:
   bridge_plugin();
   virtual ~bridge_plugin();

   APPBASE_PLUGIN_REQUIRES((chain_plugin))
   virtual void set_program_options(options_description&, options_description& cfg) override;

   void plugin_initialize(const variables_map& options);
   void plugin_startup();
   void plugin_shutdown();

private:
   std::unique_ptr<class bridge_plugin_impl> my;
};

enum Status {
   FailureOnVerification,
   SuccessOnVerification,
   UnderVerification,
   AwaitVerification,
};

struct bridge_blocks {
   block_id_type                             id;
   block_state                               bls;
};

struct bridge_change_schedule {
   uint32_t                                 block_num = 0; // the block has new producer schedule
   incremental_merkle                       imcre_merkle;
   std::vector<block_state>                 bs;
   uint8_t                                  status = 0;
   digest_type                              legacy_schedule_hash;
   producer_authority_schedule              schedule;
};

struct bridge_prove_action {
   uint32_t                                 block_num = 0; // the block has transfer action
   action                                   act;
   action_receipt                           receipt;
   std::vector<action_receipt>              act_receipts;
   block_id_type                            act_receipt_digest;
   incremental_merkle                       imcre_merkle;
   std::vector<block_state>                 bs;
   uint8_t                                  status = 0;
   transaction_id_type                      trx_id;
};

struct action_transfer {
   account_name                             from;
   account_name                             to;
   asset                                    quantity;
   string                                   memo;
};

}

FC_REFLECT( eosio::bridge_blocks, (id)(bls) )
FC_REFLECT( eosio::action_transfer, (from)(to)(quantity)(memo) )
FC_REFLECT( eosio::bridge_change_schedule, (block_num)(bs)(status) )
FC_REFLECT( eosio::bridge_prove_action, (block_num)(act)(receipt)(act_receipts)(act_receipt_digest)(imcre_merkle)(bs)(status) )
