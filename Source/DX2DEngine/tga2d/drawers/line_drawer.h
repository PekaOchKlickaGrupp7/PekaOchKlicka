#pragma once
#include "math/Vector2.h"
#include "render/render_common.h"
#include "shaders/Shader.h"

namespace DX2D
{
    struct LineBufferType
    {
        Vector2f myPosition;
    };

    class CRenderObjectLine;
    class CDirectEngine;
    class CLineDrawer : public CShader
    {
    public:
        CLineDrawer(CEngine* anEngine);
        ~CLineDrawer();
        bool Init() override;
        void Draw(CRenderObjectLine* aObject);
    private:
        CLineDrawer &operator =( const CLineDrawer &anOther ) = delete;
        bool CreateInputLayout(ID3D10Blob* aVS) override;
        bool InitShaders();
        void CreateBuffer();

        void SetShaderParameters(CRenderObjectLine* aObject);
        void UpdateVertexes(CRenderObjectLine* aObject);
        ID3D11Buffer *myVertexBuffer;    // global
        SSimpleVertex myVerticies[2];
        CDirectEngine& myEngine;
    };
}