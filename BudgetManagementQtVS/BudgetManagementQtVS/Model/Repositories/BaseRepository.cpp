#include <Model/Repositories/BaseRepository.h>

BaseRepository::BaseRepository() : database(DatabaseManager::instance().database()) {}
