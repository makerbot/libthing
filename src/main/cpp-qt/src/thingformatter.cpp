#include "thingformatter.h"
#include "partlibrary.h"
#include "objtree/transformnode.h"
#include "objtree/meshnode.h"

Json::Value xformToJson(const QMatrix4x4& xform) {
    Json::Value matrix(Json::arrayValue);

    qreal matData[16];
    xform.copyDataTo(matData);

    Json::Value row1(Json::arrayValue);
    row1.append(matData[0]);
    row1.append(matData[1]);
    row1.append(matData[2]);
    row1.append(matData[3]);
    Json::Value row2(Json::arrayValue);
    row2.append(matData[4]);
    row2.append(matData[5]);
    row2.append(matData[6]);
    row2.append(matData[7]);
    Json::Value row3(Json::arrayValue);
    row3.append(matData[8]);
    row3.append(matData[9]);
    row3.append(matData[10]);
    row3.append(matData[11]);
    Json::Value row4(Json::arrayValue);
    row4.append(matData[12]);
    row4.append(matData[13]);
    row4.append(matData[14]);
    row4.append(matData[15]);

    matrix.append(row1);
    matrix.append(row2);
    matrix.append(row3);
    matrix.append(row4);
    return matrix;
}

Mesh* ThingFormatter::readMesh(QFile &inf)
{
    throw -1;
}

void ThingFormatter::writeMesh(QFile& outf, const Mesh& mesh)
{
    throw -1;
}

void ThingFormatter::writeMesh(QFile& outf, const SceneNode& node)
{
    qDebug() << "Saving as a .thing";

    // for now we want to save to a folder, so our outfile must be a folder
    QDir outDir(QString(outf.fileName()+".dir"));
    outDir.mkpath(".");
    qDebug() << outf.fileName() << ".dir exists=" << QDir(QString(outf.fileName()+".dir")).exists();

    // how do we want to do this?
    //
    // create a json object for the manifest
    // traverse the scene graph (DF) like in stl ascii writer
    //   track transformation
    //   if it's a mesh node
    //     do lookup on original file (using partslib) ?
    //     write new file into folder with same name as orig file? using appropriate formatter.
    //     OR copy old file?
    //

    Json::Value manifest;

    // our current namespace
    manifest["namespace"] = Json::Value("http://spec.makerbot.com/ns/thing.0.1.1.1");

    // this should be filled from the user properties object
    manifest["attribution"]["author"] = Json::Value("Makerbot Industries");
    manifest["attribution"]["license"] = Json::Value("Internal Use Only");

    // for now, there are always these two methods of construction
    manifest["construction"] = Json::Value(Json::arrayValue);
    manifest["construction"].append(Json::Value("PlasticA"));
    manifest["construction"].append(Json::Value("PlasticB"));

    // these will be filled in as we go
    manifest["objects"] = Json::Value(Json::arrayValue);
    manifest["transformations"] = Json::Value();
    manifest["instances"] = Json::Value();

    int instanceCounter = 0;

    QStack<const SceneNode*> nodes;
    QStack<QMatrix4x4> xforms;

    nodes.push(&node);
    xforms.push(QMatrix4x4());
    xforms.top().setToIdentity();

    // do traversal
    while(!nodes.empty()) {
        const SceneNode *node = nodes.pop();
        QMatrix4x4 newTrans = xforms.pop();

        const TransformNode* transNode = dynamic_cast<const TransformNode*>(node);
        if(transNode != 0)
            newTrans *= transNode->matrix();

        const MeshNode* meshNode = dynamic_cast<const MeshNode*>(node);
        if(meshNode != 0) {

            QString oldFile = meshNode->mesh().filename();
            QString fileName = QFileInfo(oldFile).fileName();
            QString newFile(outDir.path()+"/"+fileName);

            Json::Value jsonName = Json::Value(fileName.toStdString());
            bool found = false;
            // check for the mesh already having been added
            for(Json::ArrayIndex i = 0; i < manifest["objects"].size(); i++) {
                if(jsonName == manifest["objects"].get(i, Json::Value(Json::nullValue)))
                    found = true;
            }
            if(!found) {
                QFile::copy(oldFile, newFile);
                manifest["objects"].append(jsonName);
            }

            // In either case, add a new Transform and a new instance
            instanceCounter++;
            QString xformName = QString("xform%1").arg(instanceCounter);

            manifest["transformations"]
                    [xformName.toStdString()]
                    ["matrix"] = xformToJson(newTrans);

            QString instanceName = QString("inst%1").arg(instanceCounter);
            std::string instNameStd = instanceName.toStdString();
            manifest["instances"]
                    [instNameStd]
                    ["object"] = Json::Value(fileName.toStdString());
            manifest["instances"]
                    [instNameStd]
                    ["scale"] = Json::Value("mm");
            manifest["instances"]
                    [instNameStd]
                    ["xform"] = Json::Value(xformName.toStdString());
            manifest["instances"]
                    [instNameStd]
                    ["construction"] = Json::Value("PlasticA");
        }

        foreach(SceneNode* sn, node->children()) {
            nodes.push(sn);
            xforms.push(newTrans);
        }
    }

    if(instanceCounter <= 0) {
        // Error!
        return;
    }

    // write out manifest
    Json::StyledWriter writer;
    QFile maniFile(outDir.path()+"/"+"manifest.json");
    maniFile.open(QIODevice::WriteOnly | QIODevice::Text);
    qDebug() << outDir.path()+"/"+"manifest.json exists=" << maniFile.exists();
    QTextStream maniStream(&maniFile);
    maniStream << writer.write(manifest).c_str();
    maniStream.flush();
    maniFile.close();
}
