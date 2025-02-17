#include "Mesh.h"

Mesh::Mesh(const std::string& _filename): nbIndices(0)
{
    filename = _filename;
}

void Mesh::Load()
{
    if (isLoaded)
        return;

    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        return;
    }
    std::string directory = filename.substr(0, filename.find_last_of('/'));

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[i];
        ProcessMesh(mesh);
    }

    nbIndices = static_cast<uint16_t>(indices.size());
    BindBuffers(vertices, indices);
    vertices.clear();
    indices.clear();
    isLoaded = true;
}

void Mesh::Unload()
{
    attributes.Unload();
}

void Mesh::BindBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    buffer.Load();
    attributes.Load();
    attributes.Bind();

    buffer.Bind(vertices, indices);
    attributes.Setup();
    attributes.Reset();
    buffer.Reset();
}

void Mesh::ProcessMesh(const aiMesh* mesh)
{
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // Position du sommet
        if (mesh->HasPositions())
        {
            vertex.pos = vec3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            );
        }

        // Normale du sommet
        if (mesh->HasNormals())
        {
            vertex.normal = vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }

        // Coordonnees UV (premiere couche)
        if (mesh->HasTextureCoords(0))
        {
            vertex.uv = vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }

        /*if (mesh->HasTangentsAndBitangents())
        {
            vertex.tangent = vec3(
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            );

            vertex.bitangent = vec3(
                mesh->mBitangents[i].x,
                mesh->mBitangents[i].y,
                mesh->mBitangents[i].z
            );
        }*/
        else
        {
            vertex.uv = vec2(0.0f, 0.0f); // Par d�faut si pas de UV
        }

        vertices.push_back(vertex);
    }

    // Parcourir les indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
}
