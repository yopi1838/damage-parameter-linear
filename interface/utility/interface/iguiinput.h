#pragma once

namespace itasca {
    /** \brief The interface to graphics user interface input functionality
    *
    * \ingroup utility
    */
    class IGUIInput {
    public:
        /// Notifies kernel. This function blocks and may throw a quit exception.
        virtual QVariant notify(const QVariant &arg1,const QVariant &arg2,const QVariant &arg3)=0;
        /// Displays a message box to the user.
        /// \param caption message box caption
        /// \param text text that will be displayed to user
        /// \param type message box type
        /// Returns the type chosen by user. This function blocks and may throw a quit exception.
        virtual int      messageBox(const QString &caption,const QString &text,int type)=0; 
        /// Used to ask user to enter a value
        /// \param message the prompt messsage
        /// \param starting
        /// Returns user output. This function blocks and may throw a quit exception.
        virtual QString  inputBox(const QString &message,const QString &starting)=0; 
        /// If filename is null, use the current project file name.
        virtual void saveProject(const QString &filename,bool interactive,bool autoSave)=0;
        virtual void restoreProject(const QString &filename,bool interactive)=0;
        virtual QStringList getProjectMasterFileNames()=0;
    };
} // namespace itasca
// EoF

