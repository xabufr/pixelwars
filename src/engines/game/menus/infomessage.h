#ifndef INFOMESSAGE_H
#define INFOMESSAGE_H

#include <string>
class GuiItem;
class InfoMessage
{
    public:
        InfoMessage(const std::string&);
        virtual ~InfoMessage();
        static void CallbackOK(GuiItem*);
    protected:
    private:
        bool m_run;
};

#endif // INFOMESSAGE_H
