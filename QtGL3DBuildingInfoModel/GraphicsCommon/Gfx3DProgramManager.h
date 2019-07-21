#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#include <QMap>
#include <QtGui/QOpenGLShaderProgram>
#include <QMutex>

class Gfx3DProgramManager : public QObject
{
    Q_OBJECT
private:
    Gfx3DProgramManager();

public:
    ~Gfx3DProgramManager(void){}

    QOpenGLShaderProgram* ProgramLoad(const QString& p_Name, const QString& p_VertexShader, const QString& p_FragmentShader);
    inline unsigned char ProgramCount() { return m_MapShader.count(); }
    QOpenGLShaderProgram* Program(const QString& p_Name) { return m_MapShader[p_Name]; }
    static Gfx3DProgramManager* Singleton();

private:
    bool AddShader(QOpenGLShaderProgram* p_Program, const QOpenGLShader::ShaderTypeBit& p_Type, const QString& p_Source);

private:
    QMap<QString, QOpenGLShaderProgram*> m_MapShader;
    static Gfx3DProgramManager* m_Instance;
    static QMutex m_InstanceMutex;
};

#endif // PROGRAMMANAGER_H
