#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
using namespace eosio;

class [[eosio::contract]] cpuminer : public contract {
  private:
    name exec_account = "testaccount"_n;//change to your account
  public:
  using contract::contract;
  [[eosio::action]]
  void pow(uint64_t cnt,asset min_sell,asset min_get,bool ischeckbp){
    if(ischeckbp){
      check(checkbp(0,5),"no atticlab bp");
    }
    asset pow_remain=get_balance("eosiopowcoin"_n,"eosiopowcoin"_n,symbol_code("POW"));
    asset my_remain=get_balance("eosiopowcoin"_n,get_self(),symbol_code("POW"));
    std::string expair="eosiopowcoin-pow-eos";
    if(my_remain>=min_sell){
      sell_newdex_market("eosiopowcoin"_n,my_remain,expair);
    }else{
      if(pow_remain.amount*0.000025>=min_get.amount){
        minepow(cnt);
      }else{
        check(false,"no enough pow");
      }
    }
  }
  
  bool checkbp(int64_t from,int64_t to){//atticlabeosb 0-5
    auto actuals=(eosio::current_time_point().time_since_epoch()._count)/1000/1000;
    int64_t tmp = (actuals-1618482906)%(21*6);
    if(tmp >= from && tmp <= to) return true;
    else return false;
  }
  void sell_newdex_market(name contract,asset amount,std::string expair){
    std::string memo = std::string("{\"type\":\"sell-market\",\"symbol\":\"")+expair+std::string("\",\"price\":\"0.0000\",\"channel\":\"dapp\",\"ref\":\"tokenpocket\"}");
    action(
      permission_level{get_self(), "active"_n},
      contract, 
      "transfer"_n, 
      std::make_tuple(get_self(),exec_account,amount,std::string())
    ).send(); 
    action(
      permission_level{exec_account, "active"_n},
      contract, 
      "transfer"_n, 
      std::make_tuple(exec_account,name("newdexpublic"),amount,memo)
    ).send(); 
  }
  void minepow(uint64_t cnt){
    symbol eos_sym(symbol_code("EOS"),4);
    asset quantity(1,eos_sym);

    for(int i=0;i<cnt;i++){
      action(
        permission_level{get_self(), "active"_n},
        name("eosio.token"), 
        "transfer"_n, 
        std::make_tuple(get_self(),name("eosiopowcoin"),quantity,std::string())
      ).send(); 
    }
  }
  struct account {
      asset   balance;
      uint64_t primary_key()const { return balance.symbol.code().raw(); }
  };

  static asset get_balance( const name& token_contract_account, const name& owner, const symbol_code& sym_code )
  {
      typedef eosio::multi_index< "accounts"_n, account > accounts;
      accounts accountstable( token_contract_account, owner.value );
      auto it = accountstable.find(sym_code.raw());
      check(it!=accountstable.end(),"Token not found, "+owner.to_string()+", "+sym_code.to_string()+"@"+token_contract_account.to_string());                        return it->balance;
  };
};