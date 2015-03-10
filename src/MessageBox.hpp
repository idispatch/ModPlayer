#ifndef MESSAGEBOX_HPP_
#define MESSAGEBOX_HPP_

#include <bb/system/SystemProgressDialog>

class MessageBox {
    MessageBox(MessageBox const&);
    MessageBox& operator = (MessageBox const&);
private:
    bb::system::SystemProgressDialog m_progress;
public:
    MessageBox(QString const & title, QString const & body, bool enabled = false, int progress = -1)
        : m_progress(NULL)
    {
        m_progress.setModality(bb::system::SystemUiModality::Application);
        m_progress.setState(bb::system::SystemUiProgressState::Active);
        m_progress.setEmoticonsEnabled(false);
        m_progress.setIncludeRememberMe(false);
        m_progress.setTitle(title);
        m_progress.setBody(body);
        m_progress.setProgress(progress);
        m_progress.confirmButton()->setEnabled(enabled);
        show();
    }

    ~MessageBox() {}

    MessageBox& setProgress(int progress) {
        m_progress.setProgress(progress);
        m_progress.show();
        return *this;
    }

    MessageBox& setShowProgress(bool active) {
        if(active) {
            m_progress.setState(bb::system::SystemUiProgressState::Active);
        } else {
            m_progress.setState(bb::system::SystemUiProgressState::Inactive);
        }
        m_progress.show();
        return *this;
    }

    MessageBox& setActivityIndicatorVisible(bool visible) {
        m_progress.setActivityIndicatorVisible(visible);
        m_progress.show();
        return *this;
    }

    MessageBox& setBody(QString const & body) {
        m_progress.setBody(body);
        m_progress.show();
        return *this;
    }

    MessageBox& enableButton(bool enabled) {
        m_progress.confirmButton()->setEnabled(enabled);
        return *this;
    }

    MessageBox& show() {
        m_progress.show();
        return *this;
    }

    bb::system::SystemUiResult::Type run() {
        return m_progress.exec();
    }
};

#endif
