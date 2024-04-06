#include <memory>

class Widget
{
	public:
		virtual ~Widget() = default;
};

class Button : public Widget
{
};

class Text : public Widget
{
};


class WidgetFactory
{
	public:
		std::unique_ptr<Widget> create(std::string type)
		{
			if (type == "button")
				return std::make_unique<Button>();
			else if (type == "text")
				return std::make_unique<Text>();
			else
				return nullptr;
		}
};

int main()
{
	WidgetFactory factory;
	auto button = factory.create("button");
	auto text = factory.create("text");
	return 0;
}
