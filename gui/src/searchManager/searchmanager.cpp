#include "log.h"
#include "searchmanager.h"

SearchManager::SearchManager(FS& fs) : _fs(fs){
	FUN();

    this->start();
}

SearchManager::~SearchManager(){
    FUN();

    //Turn off running mode and unlock the eventloop
    this->_run = false;
    this->_s_run.release();

    //Wait max 1000ms for the run() function to exit
    bool joined = this->wait(1000);
    if (!joined){
        LOGW("[SearchManager][~] Event loop stopping took longer than 1s, killing");
    }
}

void SearchManager::run(){
	FUN();

	//Try locking the run mutex to assure the eventloop gets blocked immediately
    _s_run.try_acquire();

	_run = true;
	while(_run){

		//Try to lock the run mutex, this halts this thread
        _s_run.acquire();
        LOGD("[SearchManager][run] Waking up...");

		bool workToDo = true;
		//While there is work to do
		while(workToDo){

			//See if there is work to do
			_m_searchQueue.lock();
			workToDo = _searchQueue.size() > 0;
			_m_searchQueue.unlock();

			//If there is nothing to be done, exit
			if (!workToDo){
                LOGD("[SearchManager][run] There is nothing to be done, going to sleep...");
				continue;
			}

			//Get the last search request and empty the list, we do only process the last entry
			_m_searchQueue.lock();
			std::pair<std::string, bool> curSearch = _searchQueue.back();
			_searchQueue.clear();
			_m_searchQueue.unlock();

            //Now start searching
            auto start = std::chrono::high_resolution_clock::now();
            FSDir res = _fs.searchTree(curSearch.first, curSearch.second);
            auto stop = std::chrono::high_resolution_clock::now();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);

            //Sort the children
            res.sortChildrenRec();

			//And emit the signal
            emit resultReady(curSearch.first, res, dur.count());
		}
    }
}

void SearchManager::search(std::string search, bool matchCase){
	FUN();

	_m_searchQueue.lock();
	_searchQueue.push_back(std::pair<std::string, bool>(search, matchCase));
	_m_searchQueue.unlock();

	//Unlock the eventloop, even if it is already running, this triggers at least one rerun
    _s_run.release();
}
