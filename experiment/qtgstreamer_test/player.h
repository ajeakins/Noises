#pragma once

#include <QTimer>
#include <QTime>
#include <QGst/Pipeline>

class Player : public QObject
{
    Q_OBJECT
    
public:
    Player();
    
    ~Player();
    
    void setUri( const QString& uri);
    
    QTime position() const;
    
    void setPosition( const QTime& pos );
    
    int volume() const;
    
    QTime length() const;
    
    QGst::State state() const;
    
public Q_SLOTS:
    void play();
    
    void pause();
    
    void stop();
    
    void setVolume( int volume );
    
Q_SIGNALS:
    void positionChanged();
    
    void stateChanged();
    
private:
    void onBusMessage( const QGst::MessagePtr& message);
    
    void handlePipelineStateChange( const QGst::StateChangedMessagePtr& scm );
    
private:
    QGst::PipelinePtr m_pipeline;
    QTimer m_positionTimer;
    
};

