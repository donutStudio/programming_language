enum statement_type {initialization};

class statement {
    public:
        statement_type get_type() {
            return statement_type::initialization;
        }
};