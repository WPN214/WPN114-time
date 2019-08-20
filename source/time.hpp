#ifndef AUDIOCLOCK_HPP
#define AUDIOCLOCK_HPP

#include <wpn114audio/graph.hpp>

//=================================================================================================
class TimeNode : public Node
//=================================================================================================
{
    Q_OBJECT    

    WPN_DECLARE_DEFAULT_AUDIO_INPUT     (begin, 1)
    WPN_DECLARE_AUDIO_INPUT             (end_in, 1)
    WPN_DECLARE_AUDIO_INPUT             (duration, 1)
    WPN_DECLARE_AUDIO_INPUT             (speed, 1)
    WPN_DECLARE_AUDIO_INPUT             (wait, 1)
    WPN_DECLARE_AUDIO_INPUT             (cancel, 1)
    WPN_DECLARE_DEFAULT_AUDIO_OUTPUT    (end_out, 1)

protected:

    sample_t
    m_phase = 0,
    m_rate = 0;

    enum inputs     { begin, end_in, duration, speed, wait, cancel };
    enum outputs    { end_out };
    enum state      { off, waiting, running, ended };

    state
    m_state = off;

public:

    //---------------------------------------------------------------------------------------------
    Q_SIGNAL void
    start();

    Q_SIGNAL void
    end();

    //---------------------------------------------------------------------------------------------
    enum Duration
    //---------------------------------------------------------------------------------------------
    {
        Infinite = -1
    };

    Q_ENUM (Duration)

    //---------------------------------------------------------------------------------------------
    TimeNode()
    //---------------------------------------------------------------------------------------------
    {
        m_dispatch = Dispatch::Parallel;
    }

    //---------------------------------------------------------------------------------------------
    virtual void
    initialize(Graph::properties const& properties) override
    //---------------------------------------------------------------------------------------------
    {
        m_rate = properties.rate;
    }

    //---------------------------------------------------------------------------------------------
    virtual void
    on_rate_changed(sample_t rate) override
    //---------------------------------------------------------------------------------------------
    {
        m_rate = rate;
    }

    //---------------------------------------------------------------------------------------------
    virtual void
    rwrite(pool& inputs, pool& outputs, vector_t nframes) override
    //---------------------------------------------------------------------------------------------
    {
        auto begin      = inputs.audio[inputs::begin][0];
        auto end_in     = inputs.audio[inputs::end_in][0];
        auto wait       = inputs.audio[inputs::wait][0];
        auto duration   = inputs.audio[inputs::duration][0];
        auto cancel     = inputs.audio[inputs::cancel][0];
        auto end_out    = outputs.audio[outputs::end_out][0];

        // speed TODO
        auto phase  = m_phase;
        auto rate   = m_rate;
        auto state  = m_state;

        for (vector_t f = 0; f < nframes; ++f)
        {
            switch(state)
            {
            case state::waiting:
            {
                if (phase >= wait[f]) {
                    state = running;
                    QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
                    phase = 0;
                } else {
                    phase++;
                }

                break;
            }
            case state::running:
            {
                if (phase >= duration[f]) {
                    state = ended;
                    QMetaObject::invokeMethod(this, "end", Qt::QueuedConnection);
                    end_out[f] = 1;
                    phase = 0;
                } else if (duration[f] >= 0) {
                    // if duration is infinite, do not increment phase?
                    // to be tested
                    phase++;
                }

                break;
            }
            }

            if (cancel[f] > 0) {
                // cancel will always have the highest priority
                // we don't output or signal anything
                phase = 0;
                state = off;
            }

            else if (end_in[f] > 0) {
                // in that case, we output the end signals
                end_out[f] = 1;
                QMetaObject::invokeMethod(this, "end", Qt::QueuedConnection);
                state = ended;
                phase = 0;
            }

            else if (begin[f] > 0) {
                phase = 0;
                if (wait[f] > 0)
                     state = waiting;
                else state = running;
            }
        }

        m_phase = phase;
        m_state = state;
    }

    //---------------------------------------------------------------------------------------------
    Q_INVOKABLE qreal
    sec(qreal s) { return s*m_rate; }

    //---------------------------------------------------------------------------------------------
    Q_INVOKABLE qreal
    min(qreal m)
    //---------------------------------------------------------------------------------------------
    {
        quint64 s = floor(m);
        return (s*60.0+((m-s)*100.0))*m_rate;
    }
};

//=================================================================================================
class Automation : public TimeNode
//=================================================================================================
{
    Q_OBJECT

    WPN_DECLARE_AUDIO_INPUT (from, 1)
    WPN_DECLARE_AUDIO_INPUT (to, 1)
    WPN_DECLARE_AUDIO_OUTPUT(output, 1)

public:

    //---------------------------------------------------------------------------------------------
    Automation()
    //---------------------------------------------------------------------------------------------
    {

    }

    //---------------------------------------------------------------------------------------------
    virtual void
    rwrite(pool& inputs, pool& outputs, vector_t nframes) override
    //---------------------------------------------------------------------------------------------
    {


    }
};

//=================================================================================================
class Loop : public TimeNode
//=================================================================================================
{
    Q_OBJECT

    WPN_DECLARE_AUDIO_INPUT(times, 1)
    WPN_DECLARE_AUDIO_OUTPUT(lap, 1)

public:

    //---------------------------------------------------------------------------------------------
    Loop()
    //---------------------------------------------------------------------------------------------
    {

    }

    //---------------------------------------------------------------------------------------------
    virtual void
    rwrite(pool& inputs, pool& outputs, vector_t nframes) override
    //---------------------------------------------------------------------------------------------
    {

    }
};

#endif // AUDIOCLOCK_HPP
