# Advance-CPU-Miner-on-EOS

Change exec_account to your owner account before you use it.  
This miner could mine POW(contract:eosiopowcoin). It can check the bp (to save the cpu usage) auto sell POW on newdex if it reach specific amount. If the mining pow lower than specific amount, the transaction will failed.  

[[eosio::action]] void pow(uint64_t cnt,asset min_sell,asset min_get,bool ischeckbp)  
cnt: specific the count of mining pow in a transaction  
min_sell: specific the minimum amount of selling POW on newdex in market price. If this amount was reached, call pow will not mining pow but only sell POW on newdex.  
min_get: specific the minimum amount of one time POW mining.  
is_checkbp: If it was enabled and the bp of transaction is not atticlab, the transaction will failed.  
