#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <QThread>
#include <mutex>
#include <semaphore>
#include <deque>
#include <chrono>

#include "fs.h"

class SearchManager : public QThread
{
    Q_OBJECT
public:
    /**
     * @brief   Creates a new SearchManager searching the supplied FS
     * @param   fs              The filesystem to search in
     */
    SearchManager(FS& fs);
    ~SearchManager();

    /**
     * @brief   The eventloop runs in its own thread
     */
    void run() override;

    /**
     * @brief   Order a new search operation, the result gets emitted in resultReady();
     * @param   search          The search term to search for
     * @param   matchCase       If the casing should be matched
     */
    void                        search(std::string search, bool matchCase = false);

signals:
    
    /**
     * @brief   Gets emited once a search operation finished
     * @param   searchTerm      The 'search' argument passed to search()
     * @param   entries         A std::deque containing all results as fs_entry
     * @param   us_searched     The microseconds passed while searched
     */
    void                        resultReady(std::string searchTerm, std::deque<fs_entry> entries, uint64_t us_searched);

private:
    /**
     * @brief   The filesystem to query for the search string
     */
    FS&                         _fs;

    /**
     * @brief   A semaphore that prevents the eventloop from proceeding
     */
    std::binary_semaphore       _s_run{0};

    /**
     * @brief   The queue of search requests
     */
    std::deque<std::pair<std::string, bool>> _searchQueue;
    std::mutex                  _m_searchQueue;

    /**
     * @brief   If the eventloop should continue
     */
    bool                        _run;

};

#endif // SEARCHMANAGER_H
