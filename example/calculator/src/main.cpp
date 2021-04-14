#include "thWin32App.h"

#include <functional>
#include <array>
#include <stdexcept>

// Enable Visual Styles from Common Controls 6 (ComCtl32).
#pragma comment( linker, "/manifestdependency:\"type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
language='*'\"")

// Simple example calculator implementation.
// Operates on 32 ints. No under/overflow checks.
// todo: maybe add +/- sign button.
class CalculatorLogic
{
    private:
        enum class Operation
        {
            None, Add, Sub, Mul, Div
        };

    public:
        void clear()
        {
            m_operation = Operation::None;
            m_previous_value = 0;
            m_dividedByZero = false;
        }

        void add( int a_current_value)
        {
            m_previous_value = a_current_value;
            m_operation = Operation::Add;
        }

        void minus( int a_current_value)
        {
            m_previous_value = a_current_value;
            m_operation = Operation::Sub;
        }

        void multiply( int a_current_value)
        {
            m_previous_value = a_current_value;
            m_operation = Operation::Mul;
        }

        void divide( int a_current_value)
        {
            m_previous_value = a_current_value;
            m_operation = Operation::Div;
        }

        bool result( int a_current_value, int & a_result)
        {
            switch ( m_operation)
            {
                case Operation::Add:
                {
                    a_result = m_previous_value + a_current_value;
                    break;
                }
                case Operation::Sub:
                {
                    a_result = m_previous_value - a_current_value;
                    break;
                }
                case Operation::Mul:
                {
                    a_result = m_previous_value * a_current_value;
                    break;
                }
                case Operation::Div:
                {
                    if ( 0 == a_current_value)
                    {
                        m_dividedByZero = true;
                        m_operation = Operation::None;
                        return false;
                    }

                    a_result = m_previous_value / a_current_value;
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        bool isDivByZero()
        {
            return m_dividedByZero;
        }

    private:
        Operation   m_operation{ Operation::None};
        int         m_previous_value{ 0};
        bool        m_dividedByZero{ false};
};

class CalculatorGui : public thWin32App
{
    public:
        CalculatorGui();

    private:
        CalculatorLogic             m_logic;

        std::unique_ptr< thForm>    m_MainWindow;
        
        std::unique_ptr< thEditBox> m_InputEditBox;

        std::unique_ptr< thButton>  m_Number0Button;
        std::unique_ptr< thButton>  m_Number1Button;
        std::unique_ptr< thButton>  m_Number2Button;
        std::unique_ptr< thButton>  m_Number3Button;
        std::unique_ptr< thButton>  m_Number4Button;
        std::unique_ptr< thButton>  m_Number5Button;
        std::unique_ptr< thButton>  m_Number6Button;
        std::unique_ptr< thButton>  m_Number7Button;
        std::unique_ptr< thButton>  m_Number8Button;
        std::unique_ptr< thButton>  m_Number9Button;
        
        std::unique_ptr< thButton>  m_PlusButton;
        std::unique_ptr< thButton>  m_MinusButton;
        std::unique_ptr< thButton>  m_MultiplyButton;
        std::unique_ptr< thButton>  m_DivisionButton;

        std::unique_ptr< thButton>  m_ResultButton;
        std::unique_ptr< thButton>  m_ClearButton;
        std::unique_ptr< thButton>  m_DeleteButton;

        // Generalized button onClick event callback.
        thResult_t Button_onClick( thObject *, thEventParams_t);

        // Wrap stoi exceptions.
        bool textToInt( int & a_result)
        {
            try
            {
                a_result = std::stoi( m_InputEditBox->Text);
                return true;
            }
            catch(...)
            {
                m_InputEditBox->Text = L"Internal error";
            
                m_PlusButton->Disable();
                m_MinusButton->Disable();
                m_MultiplyButton->Disable();
                m_DivisionButton->Disable();
                m_ResultButton->Disable();
                m_DeleteButton->Disable();

                m_Number0Button->Disable();
                m_Number1Button->Disable();
                m_Number2Button->Disable();
                m_Number3Button->Disable();
                m_Number4Button->Disable();
                m_Number5Button->Disable();
                m_Number6Button->Disable();
                m_Number7Button->Disable();
                m_Number8Button->Disable();
                m_Number9Button->Disable();
            }

            return false;
        }
};

// Windows application entry point.
#if TH_DEBUG_LEVEL == 0
int WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
#else
 int main()
#endif
{
    CalculatorGui calculator;

    return calculator.Run();
}

// On creation allocate all visual components.
CalculatorGui::CalculatorGui()
{
    // Create Main Window.
    {
        m_MainWindow = std::make_unique< thForm>();

        m_MainWindow->Text = L"Calculator example";
        m_MainWindow->Width = 395;
        m_MainWindow->Height = 300;
        m_MainWindow->Resizable = false;
        m_MainWindow->OnDestroy = []( thObject *, thEventParams_t)
            {
                constexpr const thResult_t quit_application = 1;
                return quit_application;
            };
    }

    // Create input edit box.
    {
        constexpr int x_position = 5;
        constexpr int y_position = 5;

        m_InputEditBox = std::make_unique< thEditBox>( m_MainWindow.get(), x_position, y_position);

        m_InputEditBox->Width = m_MainWindow->Width - 2 * x_position;
        m_InputEditBox->Height = 50;
        m_InputEditBox->Font.SetSize(30);
        m_InputEditBox->Disable();
        m_InputEditBox->Text = L"0";
    }

    // Create Button 7.
    {
        const int x_position = 5;
        const int y_position = 60;

        m_Number7Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);

        m_Number7Button->Width = 75;
        m_Number7Button->Height = 50;
        m_Number7Button->Font.SetSize(15);
        m_Number7Button->Text = L"7";
        m_Number7Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2 );
    }

    // Create Button 8.
    {
        const int x_position = 5 + m_Number7Button->Width;
        const int y_position = 60;

        m_Number8Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);

        m_Number8Button->Width = 75;
        m_Number8Button->Height = 50;
        m_Number8Button->Font.SetSize(15);
        m_Number8Button->Text = L"8";
        m_Number8Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }
    
    // Create Button 9.
    {
        const int x_position = 5 + m_Number8Button->Width + m_Number7Button->Width;
        const int y_position = 60;

        m_Number9Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_Number9Button->Width = 75;
        m_Number9Button->Height = 50;
        m_Number9Button->Font.SetSize(15);
        m_Number9Button->Text = L"9";
        m_Number9Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }
    
    // Create Button 4.
    {
        const int x_position = 5;
        const int y_position = 60 + 50;

        m_Number4Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_Number4Button->Width = 75;
        m_Number4Button->Height = 50;
        m_Number4Button->Font.SetSize(15);
        m_Number4Button->Text = L"4";
        m_Number4Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }
    
    // Create Button 5.
    {
        const int x_position = 5 + m_Number4Button->Width;
        const int y_position = 60 + 50;

        m_Number5Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_Number5Button->Width = 75;
        m_Number5Button->Height = 50;
        m_Number5Button->Font.SetSize(15);
        m_Number5Button->Text = L"5";

        m_Number5Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }
    
    // Create Button 6.
    {
        const int x_position = 5 + m_Number4Button->Width + m_Number5Button->Width;
        const int y_position = 60 + 50;

        m_Number6Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_Number6Button->Width = 75;
        m_Number6Button->Height = 50;
        m_Number6Button->Font.SetSize(15);
        m_Number6Button->Text = L"6";

        m_Number6Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }

    // Create Button 1.
    {
        const int x_position = 5;
        const int y_position = 60 + 50 + 50;

        m_Number1Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_Number1Button->Width = 75;
        m_Number1Button->Height = 50;
        m_Number1Button->Font.SetSize(15);
        m_Number1Button->Text = L"1";

        m_Number1Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }
    
    // Create Button 2.
    {
        const int x_position = 5 + m_Number4Button->Width;
        const int y_position = 60 + 50 + 50;

        m_Number2Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_Number2Button->Width = 75;
        m_Number2Button->Height = 50;
        m_Number2Button->Font.SetSize(15);
        m_Number2Button->Text = L"2";

        m_Number2Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }
    
    // Create Button 3.
    {
        const int x_position = 5 + m_Number4Button->Width + m_Number5Button->Width;
        const int y_position = 60 + 50 + 50;

        m_Number3Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_Number3Button->Width = 75;
        m_Number3Button->Height = 50;
        m_Number3Button->Font.SetSize(15);
        m_Number3Button->Text = L"3";

        m_Number3Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }

    // Create Button 0.
    {
        const int x_position = 5 + m_Number4Button->Width;
        const int y_position = 60 + 50 + 50 + 50;

        m_Number0Button = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_Number0Button->Width = 75;
        m_Number0Button->Height = 50;
        m_Number0Button->Font.SetSize(15);
        m_Number0Button->Text = L"0";

        m_Number0Button->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }
    
    // Create plus Button.
    {
        const int x_position = 15 + 3* m_Number4Button->Width;
        const int y_position = 60;

        m_PlusButton = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_PlusButton->Width = 75;
        m_PlusButton->Height = 50;
        m_PlusButton->Font.SetSize(20);
        m_PlusButton->Text = L"+";

        m_PlusButton->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }

    // Create minus Button.
    {
        const int x_position = 15 + 3* m_Number4Button->Width;
        const int y_position = 60 + 50;

        m_MinusButton = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_MinusButton->Width = 75;
        m_MinusButton->Height = 50;
        m_MinusButton->Font.SetSize(20);
        m_MinusButton->Text = L"-";

        m_MinusButton->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }

    // Create multiply Button.
    {
        const int x_position = 15 + 3* m_Number4Button->Width;
        const int y_position = 60 + 50 + 50;

        m_MultiplyButton = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_MultiplyButton->Width = 75;
        m_MultiplyButton->Height = 50;
        m_MultiplyButton->Font.SetSize(20);
        m_MultiplyButton->Text = L"X";

        m_MultiplyButton->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }

    // Create division Button.
    {
        const int x_position = 15 + 3* m_Number4Button->Width;
        const int y_position = 60 + 50 + 50 + 50;

        m_DivisionButton = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_DivisionButton->Width = 75;
        m_DivisionButton->Height = 50;
        m_DivisionButton->Font.SetSize(20);
        m_DivisionButton->Text = L"÷";

        m_DivisionButton->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }

    // Create clear Button.
    {
        const int x_position = 15 + 4* m_Number4Button->Width;
        const int y_position = 60;

        m_ClearButton = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_ClearButton->Width = 75;
        m_ClearButton->Height = 50;
        m_ClearButton->Font.SetSize(20);
        m_ClearButton->Text = L"CLR";

        m_ClearButton->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }

    // Create minus Button.
    {
        const int x_position = 15 + 4* m_Number4Button->Width;
        const int y_position = 60 + 50;

        m_DeleteButton = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_DeleteButton->Width = 75;
        m_DeleteButton->Height = 50;
        m_DeleteButton->Font.SetSize(20);
        m_DeleteButton->Text = L"DEL";

        m_DeleteButton->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }

    // Create result Button.
    {
        const int x_position = 15 + 4* m_Number4Button->Width;
        const int y_position = 60 + 50 + 50 + 50;

        m_ResultButton = std::make_unique< thButton>( m_MainWindow.get(), x_position, y_position);
    
        m_ResultButton->Width = 75;
        m_ResultButton->Height = 50;
        m_ResultButton->Font.SetSize(20);
        m_ResultButton->Text = L"=";

        m_ResultButton->OnClick = std::bind( &CalculatorGui::Button_onClick, this, std::placeholders::_1, std::placeholders::_2);
    }

    m_MainWindow->Show(); // Show main window.
}

thResult_t CalculatorGui::Button_onClick( thObject * sender, thEventParams_t)
{
    thButton * pressed_button = dynamic_cast< thButton*> ( sender);

    const size_t number_of_buttons{ 10U};

    // Group together number buttons.
    const std::array< const thButton *,number_of_buttons> number_buttons
    {
        m_Number0Button.get(), m_Number1Button.get(), m_Number2Button.get(),
        m_Number3Button.get(), m_Number4Button.get(), m_Number5Button.get(),
        m_Number6Button.get(), m_Number7Button.get(), m_Number8Button.get(),
        m_Number9Button.get()
    };

    // Check number button first.
    for ( size_t i = 0; i < number_of_buttons; ++i)
    {
        if ( number_buttons[ i] == pressed_button)
        {
            if ( m_logic.isDivByZero())
            {
                m_logic.clear();

                m_InputEditBox->Text = std::to_wstring( i);

                m_PlusButton->Enable();
                m_MinusButton->Enable();
                m_MultiplyButton->Enable();
                m_DivisionButton->Enable();
                m_ResultButton->Enable();
                m_DeleteButton->Enable();
            }
            else
            {
                std::wstring current_text = m_InputEditBox->Text;

                if ( !current_text.compare( L"0"))
                {
                    m_InputEditBox->Text = std::to_wstring( i);
                }
                else
                {
                    m_InputEditBox->Text += std::to_wstring( i);
                }
            }

            return 1;
        }
    }

    // Check functinal buttons.

    // Plus
    if ( m_PlusButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        m_logic.add( current_value);
        m_InputEditBox->Text = L"0";
        return 1;
    }

    // Minus
    if ( m_MinusButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        m_logic.minus( current_value);
        m_InputEditBox->Text = L"0";
        return 1;
    }

    // Multiply
    if ( m_MultiplyButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        m_logic.multiply( current_value);
        m_InputEditBox->Text = L"0";
        return 1;
    }

    // Division
    if ( m_DivisionButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        m_logic.divide( current_value);
        m_InputEditBox->Text = L"0";
        return 1;
    }

    // Result
    if ( m_ResultButton.get() == pressed_button)
    {
        int current_value = 0;

        if ( false == textToInt( current_value))
        {
            m_InputEditBox->Text = L"Exception occurred";
            return 1;
        }

        int result = 0;

        if ( true == m_logic.result( current_value, result))
        {
            m_InputEditBox->Text = std::to_wstring( result);
        }
        else
        {
            m_InputEditBox->Text = L"Division by zero";
            
            m_PlusButton->Disable();
            m_MinusButton->Disable();
            m_MultiplyButton->Disable();
            m_DivisionButton->Disable();
            m_ResultButton->Disable();
            m_DeleteButton->Disable();
        }

        return 1;
    }

    // Clear
    if ( m_ClearButton.get() == pressed_button)
    {
        m_logic.clear();
        m_InputEditBox->Text = L"0";

        m_PlusButton->Enable();
        m_MinusButton->Enable();
        m_MultiplyButton->Enable();
        m_DivisionButton->Enable();
        m_ResultButton->Enable();
        m_DeleteButton->Enable();

        m_Number0Button->Enable();
        m_Number1Button->Enable();
        m_Number2Button->Enable();
        m_Number3Button->Enable();
        m_Number4Button->Enable();
        m_Number5Button->Enable();
        m_Number6Button->Enable();
        m_Number7Button->Enable();
        m_Number8Button->Enable();
        m_Number9Button->Enable();

        return 1;
    }

    // Delete
    if ( m_DeleteButton.get() == pressed_button)
    {
        std::wstring current_text = m_InputEditBox->Text;

        // If there is one digit left.
        if ( 1 == current_text.size())
        {
            m_InputEditBox->Text = L"0";
        }
        // If there are 2 digit left, check if
        // '-' sign is not first.
        if ( 2 == current_text.size())
        {
            if ( L'-' == current_text.at(0))
            {
                m_InputEditBox->Text = L"0";
            }
            else
            {
                current_text.erase( current_text.size() - 1);

                m_InputEditBox->Text = current_text;
            }
        }
        else if ( current_text.size() > 1)
        {
            current_text.erase( current_text.size() - 1);

            m_InputEditBox->Text = current_text;
        }

        return 1;
    }

    return 1;
}
