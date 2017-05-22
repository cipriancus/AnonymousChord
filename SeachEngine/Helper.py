
def get_only_characters(string):
    only_numbers=''
    for iterator in string:
        if not str.isdigit(iterator):
            return only_numbers
        else:
            only_numbers=only_numbers+iterator
    return only_numbers