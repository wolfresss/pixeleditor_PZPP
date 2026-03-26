//
// Created by izakr on 22/03/2026.
//

#include "IFileManager.h"

Document IFileManager::CreateDocument(int w, int h) {
    return Document(w,h);
}

//Layer CreateLayer(int documentId, int id, std::string name, int w, int h) {
  //  return Layer(documentId, id, name, w, h);
//}

void DeleteLayer(int documentId, int id) {

}