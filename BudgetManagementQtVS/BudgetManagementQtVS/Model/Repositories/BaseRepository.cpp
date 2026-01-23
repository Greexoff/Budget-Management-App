/**
 * @file BaseRepository.cpp
 * @brief Implementation of the base repository.
 */
#include <Model/Repositories/BaseRepository.h>

 /**
  * @brief Constructor. Initializes the database reference from the DatabaseManager singleton.
  */
BaseRepository::BaseRepository() : database(DatabaseManager::instance().database()) {}
