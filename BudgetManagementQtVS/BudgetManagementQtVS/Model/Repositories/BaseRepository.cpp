#include <Model/Repositories/BaseRepository.h>

/**
 * @brief Constructs BaseRepository with database connection
 */
BaseRepository::BaseRepository() : database(DatabaseManager::instance().database()) {}
